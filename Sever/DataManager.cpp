#include "pch.h"
#include "DataManager.h"
#include "PlayerInfo.h"

SINGLETON_VAR_INIT(DataManager)

size_t DataManager::m_ClientCount = 0;
DataManager::DataManager()
{
}

DataManager::~DataManager()
{
	auto StartIter = m_ClientList.begin();
	auto EndIter = m_ClientList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if (*StartIter != nullptr)
		{
			delete *StartIter;
			*StartIter = nullptr;
		}
	}

}

void DataManager::PushClient(SocketInfo * Socket)
{
	m_ClientList.push_back(Socket);
	m_vecClient.push_back(Socket);
}

void DataManager::PushPlayer(PlayerInfo * Object)
{
	m_vecPlayerInfo.push_back(Object);
}

void DataManager::DeleteSocket(SocketInfo * Socket)
{
	size_t ClientID = -1;

	auto StartIter = m_ClientList.begin();
	auto EndIter = m_ClientList.begin();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->m_Socket == Socket->m_Socket)
		{
			ClientID = (*StartIter)->m_CliendID;
			StartIter = m_ClientList.erase(StartIter);
			delete *StartIter;
		}
		else
			StartIter++;
	}

	auto StartIter1 = m_vecPlayerInfo.begin();
	auto EndIter1 = m_vecPlayerInfo.begin();

	for (; StartIter1 != EndIter1;)
	{
		if ((*StartIter1)->m_ID == ClientID)
		{
			StartIter1 = m_vecPlayerInfo.erase(StartIter1);
			delete *StartIter1;
		}
		else
			StartIter++;
	}
}

void DataManager::SendAllClient(SocketInfo * Socket, IO_Data* Data)
{
	for (auto CurSocket : m_ClientList)
	{
		if (CurSocket->m_Socket == Socket->m_Socket)
			continue;

	}
}

void DataManager::MessageProcess(SocketInfo * Socket, IO_Data * Data)
{
	m_State = MessageManager::Get()->RecvMsg(Socket, Data);

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
	case SST_NONE:
		break;
	}
}

SocketInfo * DataManager::FindSocket(size_t ClientID)
{
	return m_vecClient[ClientID];
}
