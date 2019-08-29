#include "stdafx.h"
#include "DataManager.h"

JEONG_USING
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

}

SocketInfo * DataManager::FindSocket(size_t ClientID)
{
	return m_vecClient[ClientID];
}
