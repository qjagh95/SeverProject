#pragma once

JEONG_USING

class PlayerInfo
{
public:
	Vector3 m_Pos;
	Vector4 m_Color;
	float m_Scale;
	size_t m_ID;

public:
	PlayerInfo();
	~PlayerInfo();
};

