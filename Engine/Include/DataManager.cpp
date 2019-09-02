#include "stdafx.h"
#include "DataManager.h"
#include "Player_Com.h"

JEONG_USING
SINGLETON_VAR_INIT(DataManager)

size_t DataManager::m_ClientCount = 0;
size_t DataManager::m_OtherPlayerCount = 0;

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
	Safe_Delete_VecList(m_vecClient);
	Safe_Delete_VecList(m_vecMainPlayerInfo);
}

void DataManager::PushClient(SocketInfo * Socket)
{
	m_vecClient.push_back(Socket);
}

void DataManager::DeleteSocket(SocketInfo * Socket)
{
	size_t ClientID = -1;

	closesocket(Socket->m_Socket);

	for (size_t i = 0; i < m_vecClient.size(); i++)
	{
		if (m_vecClient[i]->m_Socket == Socket->m_Socket)
		{
			m_vecClient.erase(m_vecClient.begin() + i);
			ClientID = i;
		}
	}

}

SocketInfo * DataManager::FindClientIndex(size_t ClientID)
{
	return m_vecClient[ClientID];
}

SocketInfo * DataManager::FindClientMap(size_t Key)
{
	auto FindIter = m_ClientMap.find(Key);

	if (FindIter == m_ClientMap.find)
		return NULLPTR;

	return FindIter->second;
}

void DataManager::PushMainPlayerInfo(Player_Com * Player)
{
	PlayerInfo* newInfo = new PlayerInfo();
	newInfo->m_Color = &Player->GetRGB();
	newInfo->m_Pos = &Player->GetTransform()->GetWorldPos();
	newInfo->m_Scale = Player->GetScale();

	m_vecMainPlayerInfo.push_back(newInfo);
}

void DataManager::PushOtherPlayerInfo(Vector4 * Color, Vector3 * Pos, float * Scale)
{
	PlayerInfo* newInfo = new PlayerInfo();
	newInfo->m_Scale = Scale;
	newInfo->m_Color = Color;
	newInfo->m_Pos = Pos;
	
	m_OtherPlayerMap.insert(make_pair(m_OtherPlayerCount, newInfo));
	m_OtherPlayerCount++;
}
