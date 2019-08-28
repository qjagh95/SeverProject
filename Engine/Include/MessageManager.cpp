#include "stdafx.h"
#include "MessageManager.h"
#include "WriteMemoryStream.h"
#include "ReadMemoryStream.h"

JEONG_USING

SINGLETON_VAR_INIT(MessageManager)

MessageManager::MessageManager()
{
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

void  MessageManager::RecvNewPlayerMsg(SocketInfo * Socket, IO_Data * Data)
{
	char Buffer[BUFFERSIZE] = {};
	ReadMemoryStream Reader(Buffer); 
	DWORD Flags = 0;

	WSARecv(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, &Flags, &Data->m_Overlapped, NULLPTR);
}

void MessageManager::Send(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;
	WSASend(Socket->m_Socket, &Data->m_WsaBuf, 1, nullptr, Flags, &Data->m_Overlapped, nullptr);
}
