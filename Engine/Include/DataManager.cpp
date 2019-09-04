#include "stdafx.h"
#include "DataManager.h"
#include "Player_Com.h"

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

	closesocket(Socket->m_Socket);

	for (size_t i = 0; i < m_vecClient.size(); i++)
	{
		if (m_vecClient[i]->m_Socket == Socket->m_Socket)
			m_vecClient.erase(m_vecClient.begin() + i);
	}
	m_ClientCount--;
	m_PlayerCount--;
	//OT도 삭제처리
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

void DataManager::PushPlayerInfo(Vector4 * Color, Vector3 * Pos, size_t ClientID, float * Scale)
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