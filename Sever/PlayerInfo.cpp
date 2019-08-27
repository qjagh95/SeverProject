#include "pch.h"
#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{
	m_Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Scale = 0.0f;
}


PlayerInfo::~PlayerInfo()
{
}
