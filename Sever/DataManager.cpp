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
	auto StartIter = m_ClientList.begin();
	auto EndIter = m_ClientList.begin();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->m_Socket == Socket->m_Socket)
		{
			StartIter = m_ClientList.erase(StartIter);
			delete *StartIter;
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

}

SocketInfo * DataManager::FindSocket(size_t ClientID)
{
	return m_vecClient[ClientID];
}
