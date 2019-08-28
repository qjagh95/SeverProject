#include "stdafx.h"
#include "MessageManager.h"
#include "WriteMemoryStream.h"
#include "ReadMemoryStream.h"

JEONG_USING

SINGLETON_VAR_INIT(MessageManager)

MessageManager::MessageManager()
{
	m_State = SST_NONE;
}

MessageManager::~MessageManager()
{
}

void MessageManager::SendNewPlayerMsg(SocketInfo * Socket)
{
	WriteMemoryStream Writer;
	NewClientMessage Temp;
	Writer.Write(&Temp, sizeof(NewClientMessage));

	IO_Data* IoData = new IO_Data();
	IoData->WriteBuffer<NewClientMessage>(Writer.GetBuffer());

	Send(Socket, IoData);
}

void MessageManager::ClientMessageProcess(SocketInfo * Socket, IO_Data * Data)
{
	m_State = RecvMsg(Socket, Data);

	switch (m_State)
	{
	case SST_PLAYER_DATA:
		break;
	case SST_DELETE_EAT_OBJECT:
		break;
	}
}

void MessageManager::SeverMesageProcess(SocketInfo * Socket, IO_Data * Data)
{
	m_State = RecvMsg(Socket, Data);

	switch (m_State)
	{
	case SST_NEW_CLIENT:
		break;
	case SST_CREATE_EAT_OBJECT:
		break;
	case SST_CREATE_PLAYER:
		break;
	case SST_CREATE_OTHER_PLAYER:
		break;
	case SST_PLAYER_DATA:
		break;
	case SST_DELETE_EAT_OBJECT:
		break;
	}
}

SEVER_DATA_TYPE MessageManager::RecvMsg(SocketInfo * Socket, IO_Data * Data)
{
	char Buffer[BUFFERSIZE] = {};
	ReadMemoryStream Reader(Buffer); 
	DWORD Flags = 0;

	int getResult = WSARecv(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, &Flags, &Data->m_Overlapped, NULLPTR);
	ReadHeader(Buffer);

	if (getResult != 0)
		m_State = SST_NONE;

	return m_State;
}

void MessageManager::Send(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;
	WSASend(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, Flags, &Data->m_Overlapped, NULLPTR);
}

void MessageManager::ReadHeader(char * Buffer)
{
	memcpy(&m_State, Buffer, sizeof(Header));
}
