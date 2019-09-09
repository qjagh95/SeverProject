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

	m_ReadBuffer = new IO_Data();
	m_WriteBuffer = new IO_Data();
}

MessageManager::~MessageManager()
{
	m_Thread.detach();
	
	SAFE_DELETE(m_ReadBuffer);
	SAFE_DELETE(m_WriteBuffer);
}

void MessageManager::ClientInit()
{
	m_Thread = thread(&MessageManager::ClientMessageProcess, this);
}

void MessageManager::Client_ClientDie()
{
	IO_Data IoData;
	IoData.WriteHeader<ClientDieMessage>();

	ClientSend(&IoData);
}

bool MessageManager::Sever_SendNewPlayerMsg(SocketInfo * Socket)
{
	int RandNum = Core::Get()->RandomRange(0, 139);
	Vector4 RandColor = Vector4::AllColor[RandNum];
	Vector3 Pos = Vector3(500.0f, 500.0f, 1.0f);
	float Scale = 10.0f;

	DataManager::Get()->PushPlayerInfo(RandColor, Pos, Socket->m_CliendID, Scale);

	IO_Data* IoData = new IO_Data();
	IoData->WriteHeader<CreateMainPlayerMessage>();
	IoData->WriteBuffer<size_t>(&Socket->m_CliendID);
	IoData->WriteBuffer<Vector4>(&RandColor);
	IoData->WriteBuffer<Vector3>(&Pos);
	IoData->WriteBuffer<float>(&Scale);

	cout << Socket->m_CliendID << "번 클라이언트에게 플레이어 생성메세지 전송" << endl;

	return IOCPServerSend(Socket, IoData);
}

//새롭게 접속한 클라에 현재 접속한 클라갯수만큼 OT생성 메세지
bool MessageManager::Sever_NewClientCreateOtherPlayer(SocketInfo * Socket)
{
	IO_Data IoData;
	IoData.WriteHeader<CreateNewClientOtherPlayer>();

	cout << Socket->m_CliendID << "번 클라이언트에게 OtherPlayer 생성메세지 전송" << endl;

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
	m_State = IOCPSeverRecvMsg(Socket, Data);

	lock_guard<mutex> Mutex(m_Mutex);
	switch (m_State)
	{
	case SST_CLIENT_DIE:
		Sever_DieClient(Socket);
		break;
	case SST_PLAYER_DATA:
		break;
	case SST_DELETE_EAT_OBJECT:
		break;
	}

	//Mutex.unlock();
	return true;
}

void MessageManager::Sever_DieClient(SocketInfo* Socket)
{
	m_Mutex.lock();

	cout << Socket->m_CliendID << "번 클라이언트 종료" << endl;
	DataManager::Get()->DeleteSocket(Socket);
	m_State = SST_NONE;

	m_Mutex.unlock();
}

SEVER_DATA_TYPE MessageManager::IOCPSeverRecvMsg(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;
	SEVER_DATA_TYPE HeaderType = SST_NONE;

	ZeroMemory(&Data->m_Overlapped, sizeof(Data->m_Overlapped));
	//ZeroMemory(&Data->m_WsaBuf, sizeof(Data->m_WsaBuf));
	int a = WSARecv(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, &Flags, &Data->m_Overlapped, NULLPTR);
	int b = WSAGetLastError();

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

			//데이터가 필요하면 그냥 버퍼꺼내쓰면됨.
			switch (m_State)
			{
			case SST_CREATE_EAT_OBJECT:
				break;
			case SST_CREATE_PLAYER:
				CreateMainPlayer(ClientID, Reader);
				m_ReadBuffer->ClearBuffer();
				break;
			case SST_CONNECT_CLIENT_CREATE_OTHER_PLAYER:
				CreateOneOtherPlayer(ClientID, Reader);
				m_ReadBuffer->ClearBuffer();
				break;
			case SST_NEW_CLIENT_CREATE_OTHER_PLAYER:
				//CreateOtherPlayer();
				m_ReadBuffer->ClearBuffer();
				break;
			case SST_PLAYER_DATA:
				break;
			case SST_DELETE_EAT_OBJECT:
				break;
			case SST_CLIENT_DIE:
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
		return false;

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