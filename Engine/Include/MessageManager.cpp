#include "stdafx.h"
#include "MessageManager.h"
#include "WriteMemoryStream.h"
#include "ReadMemoryStream.h"

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
}

bool MessageManager::SendNewPlayerMsg(SocketInfo * Socket)
{
	WriteMemoryStream Writer;
	CreateMainPlayerMessage Temp;
	Writer.Write(&Temp, sizeof(CreateMainPlayerMessage));

	IO_Data IoData;
	IoData.WriteBuffer<CreateMainPlayerMessage>(Writer.GetBuffer());

	return Send(Socket, &IoData);
}

bool MessageManager::SendOtharPlayerMsg(SocketInfo * Socket)
{
	WriteMemoryStream Writer;
	CreateOtherPlayerMessage Temp;
	Writer.Write(&Temp, sizeof(CreateOtherPlayerMessage));

	IO_Data IoData;
	IoData.WriteBuffer<CreateOtherPlayerMessage>(Writer.GetBuffer());

	return Send(Socket, &IoData);
}

bool MessageManager::ClientMessageProcess(SocketInfo * Socket, IO_Data * Data)
{
	m_State = RecvMsg(Socket, Data);

	switch (m_State)
	{
	case SST_CREATE_EAT_OBJECT:
		break;
	case SST_CREATE_PLAYER:
		return CreateMainPlayer();
		break;
	case SST_CREATE_OTHER_PLAYER:
		return CreateMainPlayer();
		break;
	case SST_PLAYER_DATA:
		break;
	case SST_DELETE_EAT_OBJECT:
		break;
	}

	return false;
}

bool MessageManager::SeverMesageProcess(SocketInfo * Socket, IO_Data * Data)
{
	m_State = RecvMsg(Socket, Data);

	switch (m_State)
	{
	case SST_CREATE_EAT_OBJECT:
		break;
	case SST_CREATE_PLAYER:
		return SendNewPlayerMsg(Socket);
		break;
	case SST_CREATE_OTHER_PLAYER:
		return SendOtharPlayerMsg(Socket);
		break;
	case SST_PLAYER_DATA:
		break;
	case SST_DELETE_EAT_OBJECT:
		break;
	}

	return false;
}

SEVER_DATA_TYPE MessageManager::RecvMsg(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;
	SEVER_DATA_TYPE HeaderType = SST_NONE;

	int getResult = WSARecv(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, &Flags, &Data->m_Overlapped, NULLPTR);

	if (getResult != 0)
		m_State = HeaderType;

	memcpy(Data->m_Buffer, Data->m_WsaBuf.buf, Data->m_WsaBuf.len);
	memcpy(&HeaderType, Data->m_Buffer, sizeof(Header));
	memcpy(Data->m_Buffer, Data->m_Buffer + sizeof(Header), Data->m_WsaBuf.len - sizeof(Header));

	return m_State;
}

bool MessageManager::Send(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;
	int getResult = WSASend(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, Flags, &Data->m_Overlapped, NULLPTR);

	if (getResult != 0)
		return false;

	return true;
}

void MessageManager::ReadHeader(char * Buffer)
{
	memcpy(&m_State, Buffer, sizeof(Header));
}

bool MessageManager::CreateMainPlayer()
{

	m_State = SST_NONE;
	return true;
}

bool MessageManager::CreateOtherPlayer()
{

	m_State = SST_NONE;
	return true;
}
