#include "pch.h"
#include "DataManager.h"
#include <Core.h>

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
	Safe_Delete_VecList(m_vecEat);
}

void DataManager::PushClient(SocketInfo * Socket)
{
	m_ClientCount++;
	m_vecClient.push_back(Socket);
	m_ClientMap.insert(make_pair(Socket->m_CliendID, Socket));
}

void DataManager::DeleteSocket(SocketInfo * Socket)
{
	mutex Mutex;
	lock_guard<mutex> MyMutex(Mutex);

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

void DataManager::DeleteEat(int ID)
{
	int Index = 0;
	for (auto& CurEat : m_vecEat)
	{
		if (CurEat->ID == ID)
		{
			m_vecEat.erase(m_vecEat.begin() + Index);
			break;
		}

		Index++;
	}
}

void DataManager::CloseAll()
{
	for (auto CurClient : m_vecClient)
	{
		closesocket(CurClient->m_Socket);
		DeleteSocket(CurClient);
	}
}

void DataManager::Init()
{
	m_vecEat.reserve(10000);

	for (size_t i = 0; i < 10000; i++)
	{
		int RandIndex = Core::Get()->RandomRange(0, 140);

		EatInfo* newInfo = new EatInfo();
		newInfo->Color = Vector4::AllColor[RandIndex];
		newInfo->ID = static_cast<int>(i);

		float X = static_cast<float>(Core::Get()->RandomRange(0, 50000));
		float Y = static_cast<float>(Core::Get()->RandomRange(0, 50000));
		newInfo->Pos.x = static_cast<float>(X);
		newInfo->Pos.y = static_cast<float>(Y);
		newInfo->Pos.z = 1.0f;

		m_vecEat.push_back(newInfo);
	}
}
