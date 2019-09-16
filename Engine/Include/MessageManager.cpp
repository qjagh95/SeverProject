#include "stdafx.h"
#include "MessageManager.h"
#include "WriteMemoryStream.h"
#include "ReadMemoryStream.h"
#include "DataManager.h"
#include "Core.h"
#include "Player_Com.h"
#include "ConnectSever.h"
#include "OtharPlayer_Com.h"
#include "OTManager.h"

JEONG_USING

SINGLETON_VAR_INIT(MessageManager)

MessageManager::MessageManager()
{
	m_State = SST_NONE;
	m_CurLayer = NULLPTR;
	m_CurScene = NULLPTR;
}

MessageManager::~MessageManager()
{
	m_Thread.detach();
}
 
void MessageManager::ClientInit()
{
	m_Thread = thread(&MessageManager::ClientMessageProcess, this);
}

void MessageManager::Client_ClientDie()
{
	size_t MyID = ConnectSever::Get()->GetClientID();
	IO_Data IoData;
	IoData.WriteHeader<ClientDieMessage>();
	IoData.WriteBuffer<size_t>(&MyID);

	ClientSend(&IoData);
}

void MessageManager::Client_OtherPlayerDie(IO_Data * Data)
{
	ReadMemoryStream Reader(Data->GetBuffer(), Data->GetSize());
	size_t DeleteID = Reader.Read<size_t>();

	DataManager::Get()->DeleteOT(DeleteID);
}

bool MessageManager::Sever_SendNewPlayerMsg(SocketInfo * Socket)
{
	int RandNum = Core::Get()->RandomRange(0, 139);
	Vector4 RandColor = Vector4::AllColor[RandNum];
	Vector3 Pos = Vector3(500.0f, 500.0f, 1.0f);
	float Scale = 10.0f;

	DataManager::Get()->PushPlayerInfo(RandColor, Pos, Socket->m_CliendID, Scale);

	IO_Data IoData;
	IoData.WriteHeader<CreateMainPlayerMessage>();
	IoData.WriteBuffer<size_t>(&Socket->m_CliendID);
	IoData.WriteBuffer<Vector4>(&RandColor);
	IoData.WriteBuffer<Vector3>(&Pos);
	IoData.WriteBuffer<float>(&Scale);

	cout << Socket->m_CliendID << "번 클라이언트에게 플레이어 생성메세지 전송" << endl << endl;

	int ClientCount = DataManager::Get()->GetClientCount();
	auto getPlayerVec = DataManager::Get()->GetPlayerVec();

	IoData.WriteBuffer<int>(&ClientCount);

	if (ClientCount == 1)
		return IOCPServerSend(Socket, &IoData);

	//데이터를 보낸다
	for (auto Cur : *getPlayerVec)
	{
		if (Socket->m_CliendID == Cur->m_ClientID)
			continue;

		IoData.WriteBuffer(&Cur->m_Color, 16);
		IoData.WriteBuffer<Vector3>(&Cur->m_Pos);
		IoData.WriteBuffer<float>(&Cur->m_Scale);
		IoData.WriteBuffer<size_t>(&Cur->m_ClientID);
	}

	cout << Socket->m_CliendID << "번 클라이언트에 기존 접속한 클라이언트 갯수 : " << ClientCount - 1 << " 개 만큼 OtherPlayer 생성메세지 전송" << endl;

	return IOCPServerSend(Socket, &IoData);
}

//현재 접속한 클라에 OT생셩메세지
bool MessageManager::Sever_SendConnectClientNewOtherPlayer(SocketInfo * NewSocket)
{
	if (DataManager::Get()->GetClientCount() == 0 || DataManager::Get()->GetClientCount() == 1)
		return false;

	auto getVec = DataManager::Get()->GetPlayerVec();

	if (getVec->size() == 0 || getVec->size() == 1)
		return false;

	PlayerInfo* getInfo = getVec->at(getVec->size() - 1);

	cout << NewSocket->m_CliendID << "번 클라이언트에게 OtherPlayer 생성메세지 전송" << endl;

	for (auto Cur : *DataManager::Get()->GetClientList())
	{
		if (Cur->m_Socket == NewSocket->m_Socket)
			continue;

		IO_Data IoData;
		IoData.WriteHeader<CreateConnectClientCreateOtherPlayer>();
		IoData.WriteBuffer<size_t>(&getInfo->m_ClientID);
		IoData.WriteBuffer<Vector4>(&getInfo->m_Color);
		IoData.WriteBuffer<Vector3>(&getInfo->m_Pos);
		IoData.WriteBuffer<Vector3>(&getInfo->m_Scale);

		IOCPServerSend(Cur, &IoData);
	}

	return true;
}

bool MessageManager::SeverMesageProcess(SocketInfo * Socket, IO_Data * Data)
{
	lock_guard<mutex> Mutex(m_Mutex);
	m_State = IOCPSeverRecvMsg(Socket, Data);

	switch (m_State)
	{
	case SST_CLIENT_DIE:
		Sever_DieClient(Socket);
		break;
	case SST_PLAYER_DATA:
		break;
	case SST_DELETE_EAT_OBJECT:
		break;
	case SST_OTHER_PLAYER_DELETE:
		Client_OtherPlayerDie(Data);
		break;
	}

	return true;
}

void MessageManager::Sever_DieClient(SocketInfo* Socket)
{
	cout << Socket->m_CliendID << "번 클라이언트 종료" << endl;
	DataManager::Get()->DeleteSocket(Socket);
	m_State = SST_NONE;
}

SEVER_DATA_TYPE MessageManager::IOCPSeverRecvMsg(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;
	SEVER_DATA_TYPE HeaderType = SST_NONE;

	ZeroMemory(&Data->m_Overlapped, sizeof(Data->m_Overlapped));
	int a = WSARecv(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, &Flags, &Data->m_Overlapped, NULLPTR);
	int b = WSAGetLastError();

	if (b != WSA_IO_PENDING)
	{
		m_State = HeaderType;
		return m_State;
	}

	Data->CopyBuffer();

	if (Data->m_WsaBuf.buf == NULLPTR)
	{
		m_State = HeaderType;
		return m_State;
	}

	HeaderType = Data->ReadHeader();

	if (Data->m_WsaBuf.len == 0)
	{
		m_State = HeaderType;
		return m_State;
	}

	m_State = HeaderType;

	Data->HeaderErase();
	return HeaderType;
}

void MessageManager::ClientMessageProcess()
{
	WSAEVENT getEvent = ConnectSever::Get()->GetEventHandle();

	while (true)
	{
		if (m_CurScene == NULLPTR || m_CurLayer == NULLPTR)
			continue;

		int Event = WSAWaitForMultipleEvents(1, &getEvent, TRUE, WSA_INFINITE, FALSE);

		if (Event == WSA_WAIT_TIMEOUT || Event == WSA_WAIT_FAILED)
			continue;
		
		WSANETWORKEVENTS NetWorkEvent;
		SOCKET getSocket = *ConnectSever::Get()->GetSocket();
		WSAEnumNetworkEvents(getSocket, getEvent, &NetWorkEvent);

		if (NetWorkEvent.lNetworkEvents & FD_READ)
		{
			char Buffer[BUFFERSIZE] = { };
			recv(getSocket, Buffer, BUFFERSIZE, 0);

			ReadMemoryStream Reader = ReadMemoryStream(Buffer, BUFFERSIZE);
			m_State = Reader.Read<SEVER_DATA_TYPE>();
			size_t ClientID = Reader.Read<size_t>();

			switch (m_State)
			{
			case SST_CREATE_EAT_OBJECT:
				break;
			case SST_CREATE_PLAYER:
				CreateMainPlayer(ClientID, Reader);
				break;
			case SST_CONNECT_CLIENT_CREATE_OTHER_PLAYER:
				CreateOneOtherPlayer(ClientID, Reader);
				break;
			case SST_PLAYER_DATA:
				break;
			case SST_DELETE_EAT_OBJECT:
				break;
			}
		}
	}

	return;
}

void MessageManager::ClientSend(IO_Data * Data)
{
	auto getSocket = ConnectSever::Get()->GetSocketInfo();
	Data->CopyBuffer();

	IOCPServerSend(getSocket, Data);
}

bool MessageManager::IOCPServerSend(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;

	int getResult = WSASend(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, Flags, (LPOVERLAPPED)Data, NULLPTR);

	if (getResult != 0)
	{
		cout << "FalseCheck" << endl;
		return false;
	}

	return true;
}

bool MessageManager::IOCPSeverSendALL(SocketInfo * SameSocket, IO_Data * Data)
{
	for (auto CurClient : *DataManager::Get()->GetClientList())
	{
		if (CurClient->m_Socket == SameSocket->m_Socket)
			continue;
		
		IOCPServerSend(CurClient, Data);
	}
	return true;
}

SEVER_DATA_TYPE MessageManager::ReadHeader(char * Buffer)
{
	SEVER_DATA_TYPE HeaderType = SST_NONE;
	memcpy(&HeaderType, Buffer, sizeof(Header));

	return HeaderType;
}

bool MessageManager::CreateMainPlayer(size_t ClientID, ReadMemoryStream& Reader)
{
	Vector4 Color = Reader.Read<Vector4>();
	Vector3 Pos = Reader.Read<Vector3>();
	float Scale = Reader.Read<float>();

	GameObject* newPlayerObj = GameObject::CreateObject("Player", m_CurLayer);
	Player_Com* newPlayer = newPlayerObj->AddComponent<Player_Com>("Player");
	newPlayer->SetRGB(Color.x, Color.y, Color.z);
	newPlayer->SetScale(Scale);
	newPlayer->GetTransform()->SetWorldPos(Pos);

	m_CurScene->GetMainCamera()->SetTarget(newPlayerObj);

	ConnectSever::Get()->SetClientID(ClientID);

	SAFE_RELEASE(newPlayerObj);
	SAFE_RELEASE(newPlayer);

	int ClientCount = Reader.Read<int>();

	if (ClientCount == 1)
		return true;

	CreateOtherPlayer(ClientCount, Reader);

	m_State = SST_NONE;
	return true;
}

bool MessageManager::CreateOneOtherPlayer(size_t ClientID, ReadMemoryStream& Reader)
{
	Vector4 Color = Reader.Read<Vector4>();
	Vector3 Pos = Reader.Read<Vector3>();
	float Scale = Reader.Read<float>();

	GameObject* newOtherPlayerObj = GameObject::CreateObject("OtherPlayer", m_CurLayer);
	OtharPlayer_Com* newOther = newOtherPlayerObj->AddComponent<OtharPlayer_Com>("OtherPlayer");
	newOther->GetTransform()->SetWorldScale(Scale, Scale ,1.0f);
	newOther->GetTransform()->SetWorldPos(Pos);
	newOther->SetRGB(Color.x, Color.y, Color.z);

	OTManager::Get()->InsertOT(ClientID, newOther);

	SAFE_RELEASE(newOtherPlayerObj);
	SAFE_RELEASE(newOther);

	m_State = SST_NONE;
	return true;
}

bool MessageManager::CreateOtherPlayer(int ClientID, ReadMemoryStream & Reader)
{
	//ClientID가 클라이언트 갯수를 보낼것임 (내꺼빼고).
	for (int i = 0; i < ClientID - 1; i++)
	{
		Vector4 Color = Reader.Read<Vector4>();
		Vector3 Pos = Reader.Read<Vector3>();
		float Scale = Reader.Read<float>();
		size_t ID = Reader.Read<size_t>();

		GameObject* newOtherPlayerObj = GameObject::CreateObject("OtherPlayer", m_CurLayer);
		OtharPlayer_Com* newOther = newOtherPlayerObj->AddComponent<OtharPlayer_Com>("OtherPlayer");
		newOther->GetTransform()->SetWorldScale(Scale, Scale, 1.0f);
		newOther->GetTransform()->SetWorldPos(Pos);
		newOther->SetRGB(Color.x, Color.y, Color.z);

		OTManager::Get()->InsertOT(ID, newOther);

		SAFE_RELEASE(newOtherPlayerObj);
		SAFE_RELEASE(newOther);
	}

	return false;
}
