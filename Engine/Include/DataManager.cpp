#include "stdafx.h"
#include "DataManager.h"
#include "Player_Com.h"
#include "OtharPlayer_Com.h"

#include "OTManager.h"

JEONG_USING
SINGLETON_VAR_INIT(DataManager)

size_t DataManager::m_ClientCount = 0;
size_t DataManager::m_PlayerCount = 0;

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
	Safe_Delete_VecList(m_vecClient);
	Safe_Delete_VecList(m_vecPlayerInfo);
}

void DataManager::PushClient(SocketInfo * Socket)
{
	m_ClientCount++;
	m_vecClient.push_back(Socket);
	m_ClientMap.insert(make_pair(Socket->m_CliendID, Socket));
}

void DataManager::DeleteSocket(SocketInfo * Socket)
{
	size_t DeleteID = Socket->m_CliendID;

	for (size_t i = 0; i < m_vecClient.size(); i++)
	{
		if (m_vecClient[i]->m_Socket == Socket->m_Socket)
		{
			SAFE_DELETE(m_vecClient[i]);
			SAFE_DELETE(m_vecPlayerInfo[i]);
			m_vecClient.erase(m_vecClient.begin() + i);
			m_vecPlayerInfo.erase(m_vecPlayerInfo.begin() + i);
		}
	}
	m_PlayerCount--;
	m_ClientMap.erase(Socket->m_CliendID);
	m_PlayerMap.erase(Socket->m_CliendID);

	closesocket(Socket->m_Socket);
}

SocketInfo * DataManager::FindClientIndex(size_t ClientID)
{
	return m_vecClient[ClientID];
}

SocketInfo * DataManager::FindClientMap(size_t Key)
{
	auto FindIter = m_ClientMap.find(Key);

	if (FindIter == m_ClientMap.end())
		return NULLPTR;

	return FindIter->second;
}

PlayerInfo * DataManager::FindPlayerInfoIndex(size_t ClientID)
{
	return m_vecPlayerInfo[ClientID];
}

PlayerInfo * DataManager::FindPlayerInfoKey(size_t Key)
{
	auto FindIter = m_PlayerMap.find(Key);

	if (FindIter == m_PlayerMap.end())
		return NULLPTR;

	return FindIter->second;
}

void DataManager::PushPlayerInfo(const Vector4& Color, const Vector3& Pos, size_t ClientID, float Scale)
{
	PlayerInfo* newInfo = new PlayerInfo();
	newInfo->m_Scale = Scale;
	newInfo->m_Color = Color;
	newInfo->m_Pos = Pos;
	newInfo->m_ClientID = ClientID;
	
	m_vecPlayerInfo.push_back(newInfo);
	m_PlayerMap.insert(make_pair(ClientID, newInfo));
	m_PlayerCount++;
}