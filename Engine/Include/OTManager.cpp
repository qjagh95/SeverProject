#include "stdafx.h"
#include "OTManager.h"
#include "OtharPlayer_Com.h"

JEONG_USING
SINGLETON_VAR_INIT(OTManager)

OTManager::OTManager()
{
}


OTManager::~OTManager()
{
}

void OTManager::InsertOT(size_t Key, OtharPlayer_Com * OT)
{
	m_OTMap.insert(make_pair(Key, OT));
}

OtharPlayer_Com * OTManager::FindOT(size_t Key)
{
	auto FindIter = m_OTMap.find(Key);
	
	if (FindIter == m_OTMap.end())
		return NULLPTR;

	return FindIter->second;
}

void OTManager::DeleteOT(size_t key)
{
	auto getOT = FindOT(key);
	getOT->GetGameObject()->SetIsActive(false);

	m_OTMap.erase(key);
}

void OTManager::DeleteAll()
{
	m_OTMap.clear();
}
