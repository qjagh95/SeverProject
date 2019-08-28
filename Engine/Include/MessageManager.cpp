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

void MessageManager::SendNewPlayerMsg(SocketInfo * Socket, IO_Data * Data)
{
	WriteMemoryStream Writer;
}

char*  MessageManager::RecvNewPlayerMsg(SocketInfo * Socket, IO_Data * Data)
{
	char Buffer[BUFFERSIZE] = {};

	return nullptr;
}
