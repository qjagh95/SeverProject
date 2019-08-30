#include "stdafx.h"
#include "MessageManager.h"
#include "WriteMemoryStream.h"
#include "ReadMemoryStream.h"
#include "DataManager.h"
#include "Core.h"
#include "Player_Com.h"
#include "ConnectSever.h"

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
	SAFE_DELETE(m_ReadBuffer);
	SAFE_DELETE(m_WriteBuffer);
}

void MessageManager::ClientInit()
{
	m_Thread = thread(&MessageManager::ClientMessageProcess, this);
}

void MessageManager::Client_ClientDie()
{
	IO_Data Data;
	ClientDieMessage Message;
	Data.WriteHeader(Message);

	ClientSend(&Data);
}

bool MessageManager::Sever_SendNewPlayerMsg(SocketInfo * Socket)
{
	WriteMemoryStream Writer;
	CreateMainPlayerMessage Temp;
	Writer.Write(&Temp, sizeof(CreateMainPlayerMessage));

	IO_Data IoData;
	IoData.WriteBuffer<CreateMainPlayerMessage>(Writer.GetBuffer());

	cout << Socket->m_CliendID << "번 클라이언트에게 플레이어 생성메세지 전송" << endl;
	return IOCPServerSend(Socket, &IoData);
}

bool MessageManager::Sever_SendOtharPlayerMsg(SocketInfo * Socket)
{
	WriteMemoryStream Writer;
	CreateOtherPlayerMessage Temp;
	Writer.Write(&Temp, sizeof(CreateOtherPlayerMessage));

	IO_Data IoData;
	IoData.WriteBuffer<CreateOtherPlayerMessage>(Writer.GetBuffer());

	return IOCPServerSend(Socket, &IoData);
}

bool MessageManager::SeverMesageProcess(SocketInfo * Socket, IO_Data * Data)
{
	m_State = IOCPSeverRecvMsg(Socket, Data);

	switch (m_State)
	{
	case SST_CLIENT_DIE:
		Sever_DieClient(Socket);
		break;
	case SST_CREATE_EAT_OBJECT:
		break;
	case SST_CREATE_PLAYER:
		return Sever_SendNewPlayerMsg(Socket);
		break;
	case SST_CREATE_OTHER_PLAYER:
		return Sever_SendOtharPlayerMsg(Socket);
		break;
	case SST_PLAYER_DATA:
		break;
	case SST_DELETE_EAT_OBJECT:
		break;
	}

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

	int getResult = WSARecv(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, &Flags, &Data->m_Overlapped, NULLPTR);

	if (getResult != 0)
	{
		m_State = HeaderType;
		return m_State;
	}

	if (Data->m_WsaBuf.len == 0)
	{
		m_State = HeaderType;
		return m_State;
	}

	memcpy(Data->m_Buffer, Data->m_WsaBuf.buf, Data->m_WsaBuf.len);
	memcpy(&HeaderType, Data->m_Buffer, sizeof(Header));
	memcpy(Data->m_Buffer, Data->m_Buffer + sizeof(Header), Data->m_WsaBuf.len - sizeof(Header));

	return m_State;
}

void MessageManager::ClientMessageProcess()
{
	WSAEVENT getEvent = ConnectSever::Get()->GetEventHandle();

	while (true)
	{
		if (m_CurScene == NULLPTR || m_CurLayer == NULLPTR)
			return;

		int Event = WSAWaitForMultipleEvents(1, &getEvent, TRUE, WSA_INFINITE, FALSE);

		if (Event == WSA_WAIT_TIMEOUT || Event == WSA_WAIT_FAILED)
			continue;
		
		WSANETWORKEVENTS NetWorkEvent;
		SOCKET getSocket = *ConnectSever::Get()->GetSocket();
		WSAEnumNetworkEvents(getSocket, getEvent, &NetWorkEvent);

		if (NetWorkEvent.lNetworkEvents == FD_READ)
		{
			recv(getSocket, m_ReadBuffer->m_Buffer, BUFFERSIZE, 0);
			m_State = ReadHeader(m_ReadBuffer->m_Buffer);
			m_ReadBuffer->PullBuffer(sizeof(Header));

			//데이터가 필요하면 그냥 버퍼꺼내쓰면됨.
			switch (m_State)
			{
			case SST_CREATE_EAT_OBJECT:
				break;
			case SST_CREATE_PLAYER:
				CreateMainPlayer();
				m_ReadBuffer->ClearBuffer<Header>();
				break;
			case SST_CREATE_OTHER_PLAYER:
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

	if (Data->m_Buffer == NULLPTR)
		Data->WriteBuffer(Data->m_WsaBuf.buf, Data->m_WsaBuf.len);

	send(getSocket, Data->m_WsaBuf.buf, Data->m_WsaBuf.len, 0);
}

bool MessageManager::IOCPServerSend(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;

	int getResult = WSASend(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, Flags, &Data->m_Overlapped, NULLPTR);

	if (getResult != 0)
		return false;

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

	SAFE_RELEASE(newPlayerObj);
	SAFE_RELEASE(newPlayer);

	m_State = SST_NONE;
	return true;
}

bool MessageManager::CreateOtherPlayer()
{

	m_State = SST_NONE;
	return true;
}