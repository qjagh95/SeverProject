#include "stdafx.h"
#include "MessageManager.h"
#include "WriteMemoryStream.h"
#include "ReadMemoryStream.h"
#include "DataManager.h"
#include "Core.h"
#include "Player_Com.h"
#include "ConnectSever.h"
#include "OtharPlayer_Com.h"

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
	if (m_ReadBuffer->m_Stream.GetSize() > 0)
		SAFE_DELETE(m_ReadBuffer);

	if(m_WriteBuffer->m_Stream.GetSize() > 0)
		SAFE_DELETE(m_WriteBuffer);
}

void MessageManager::ClientInit()
{
	m_Thread = thread(&MessageManager::ClientMessageProcess, this);
}

void MessageManager::Client_ClientDie()
{
	IO_Data Data;
	Data.WriteHeader<ClientDieMessage>();

	ClientSend(&Data);
	DataManager::Get()->m_ClientCount--;
}

bool MessageManager::Sever_SendNewPlayerMsg(SocketInfo * Socket)
{
	IO_Data IoData;
	IoData.WriteHeader<CreateMainPlayerMessage>();

	cout << Socket->m_CliendID << "번 클라이언트에게 플레이어 생성메세지 전송" << endl;

	return IOCPServerSend(Socket, &IoData);
}

bool MessageManager::Sever_SendOtharPlayerMsg(SocketInfo * Socket)
{
	IO_Data IoData;
	IoData.WriteHeader<CreateOtherPlayerMessage>();

	return IOCPServerSend(Socket, &IoData);
}

bool MessageManager::Sever_SendConnectClientNewOtherPlayer(SocketInfo * NewSocket, PlayerInfo * Info)
{
	IO_Data IoData;
	IoData.WriteHeader<CreateOtherPlayerMessage>();

	size_t ClientSize = DataManager::Get()->GetClientCount() - 1;
	IoData.WriteBuffer<size_t>(&ClientSize);

	for (auto Cur : *DataManager::Get()->GetClientList())
	{
		if (Cur->m_Socket == NewSocket->m_Socket)
			continue;

		IoData.WriteBuffer<Vector3>(Info->m_Color);
		IoData.WriteBuffer<Vector3>(Info->m_Pos);
		IoData.WriteBuffer<Vector3>(Info->m_Scale);

		IOCPServerSend(Cur, &IoData);
	}
	return true;
}

bool MessageManager::SeverMesageProcess(SocketInfo * Socket, IO_Data * Data)
{
	m_Mutex.lock();

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
	}

	m_Mutex.unlock();

	return false;
}

void MessageManager::Sever_DieClient(SocketInfo* Socket)
{
	DataManager::Get()->DeleteSocket(Socket);
	DataManager::m_ClientCount--;

	m_State = SST_NONE;
}

SEVER_DATA_TYPE MessageManager::IOCPSeverRecvMsg(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;
	SEVER_DATA_TYPE HeaderType = SST_NONE;

	WSARecv(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, &Flags, &Data->m_Overlapped, NULLPTR);
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
			m_ReadBuffer->WriteBuffer(Buffer, BUFFERSIZE);

			m_State = ReadHeader(Buffer);
			m_ReadBuffer->HeaderErase();

			//데이터가 필요하면 그냥 버퍼꺼내쓰면됨.
			switch (m_State)
			{
			case SST_CREATE_EAT_OBJECT:
				break;
			case SST_CREATE_PLAYER:
				CreateMainPlayer();
				m_ReadBuffer->ClearBuffer();
				break;
			case SST_CREATE_OTHER_PLAYER:
				CreateOtherPlayer();
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
	SOCKET getSocket = *ConnectSever::Get()->GetSocket();

	send(getSocket, Data->m_Stream.m_WriteBuffer, Data->m_Stream.GetSize(), 0);
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

bool MessageManager::CreateMainPlayer()
{
	GameObject* newPlayerObj = GameObject::CreateObject("Player", m_CurLayer);
	Player_Com* newPlayer = newPlayerObj->AddComponent<Player_Com>("Player");
	newPlayer->SetScale(10.0f);
	m_CurScene->GetMainCamera()->SetTarget(newPlayerObj);

	DataManager::Get()->PushMainPlayerInfo(newPlayer);

	SAFE_RELEASE(newPlayerObj);
	SAFE_RELEASE(newPlayer);

	m_State = SST_NONE;
	return true;
}

bool MessageManager::CreateOtherPlayer()
{
	ReadMemoryStream Reader(m_ReadBuffer->GetBuffer(), m_ReadBuffer->GetSize());
	
	size_t Count = Reader.Read<size_t>();

	for (size_t i = 0; i < Count; i++)
	{
		Vector4 Color = Reader.Read<Vector4>();
		Vector3 Pos = Reader.Read<Vector3>();
		float Scale = Reader.Read<float>();

		GameObject* newOtherPlayerObj = GameObject::CreateObject("OtherPlayer", m_CurLayer);
		OtharPlayer_Com* newOther = newOtherPlayerObj->AddComponent<OtharPlayer_Com>("OtherPlayer");
		newOther->SetScale(Scale);
		newOtherPlayerObj->GetTransform()->SetWorldPos(Pos);
		newOther->SetRGB(Color.x, Color.y, Color.z);

		SAFE_RELEASE(newOtherPlayerObj);
		SAFE_RELEASE(newOther);
	}

	m_State = SST_NONE;
	return true;
}