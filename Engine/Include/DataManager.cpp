#include "stdafx.h"
#include "DataManager.h"
#include "Player_Com.h"

JEONG_USING
SINGLETON_VAR_INIT(DataManager)

size_t DataManager::m_ClientCount = 0;
DataManager::DataManager()
{
}

DataManager::~DataManager()
{
	SAFE_RELEASE(m_Player);
	Safe_Delete_VecList(m_vecClient);
	Safe_Delete_VecList(m_vecPlayerInfo);

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

SocketInfo * DataManager::FindSocket(size_t ClientID)
{
	return m_vecClient[ClientID];
}

void DataManager::SetPlayerObject(GameObject * Player)
{
	m_PlayerObject = Player;
	m_Player = m_PlayerObject->FindComponentFromType<Player_Com>(CT_PLAYER);
}

void DataManager::PushInfo(Player_Com * Player)
{
	PlayerInfo* newInfo = new PlayerInfo();
	newInfo->m_Color = &Player->GetRGB();
	newInfo->m_Pos = &Player->GetTransform()->GetWorldPos();
	newInfo->m_Scale = Player->GetScale();

	m_vecPlayerInfo.push_back(newInfo);
}
