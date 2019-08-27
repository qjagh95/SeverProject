#pragma once
JEONG_BEGIN

class JEONG_DLL Header
{
public:
	SEVER_DATA_TYPE m_Type;

public:
	Header();
	~Header();
};

//�����δ� �����ų� �޴� ����� ��Ȯ�ϰ� �ϱ����� �޼��� Ŭ����ȭ
class JEONG_DLL PlayerCreateMessage
{
public:
	Header m_Header;
	Vector2 PosPadding;
	float ScalePadding;

	PlayerCreateMessage()
	{
		m_Header.m_Type = SST_CREATE_PLAYER;

	}
};

class JEONG_DLL CreateOtherPlayerMessage
{
public:
	Header m_Header;

	CreateOtherPlayerMessage()
	{
		m_Header.m_Type = SST_CREATE_OTHER_PLAYER;
	}
};

class JEONG_DLL CreateEatObjectMessage
{
public:
	Header m_Header;

	CreateEatObjectMessage()
	{
		m_Header.m_Type = SST_CREATE_EAT_OBJECT;
	}
};

class JEONG_DLL SendPlayerData
{
public:
	Header m_Header;

	SendPlayerData()
	{
		m_Header.m_Type = SST_PLAYER_DATA;
	}
};

class JEONG_DLL DeleteEatObject
{
public:
	Header m_Header;

	DeleteEatObject()
	{
		m_Header.m_Type = SST_DELETE_EAT_OBJECT;
	}
};

JEONG_END