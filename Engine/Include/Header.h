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
class JEONG_DLL MainPlayerCreateMessage
{
public:
	Header Header;

	MainPlayerCreateMessage()
	{
		Header.m_Type = SST_NEW_CLIENT;
	}
};

class JEONG_DLL MainPlayerCreateMessage
{
public:
	Header Header;
	Vector2 Pos;
	Vector4 Color;
	float Scale;

	MainPlayerCreateMessage()
	{
		Header.m_Type = SST_CREATE_PLAYER;

	}
};

class JEONG_DLL CreateOtherPlayerMessage
{
public:
	Header Header;
	Vector2 Pos;
	Vector4 Color;
	float Scale;

	CreateOtherPlayerMessage()
	{
		Header.m_Type = SST_CREATE_OTHER_PLAYER;
	}
};

class JEONG_DLL CreateEatObjectMessage
{
public:
	Header Header;

	CreateEatObjectMessage()
	{
		Header.m_Type = SST_CREATE_EAT_OBJECT;
	}
};

class JEONG_DLL SendPlayerData
{
public:
	Header Header;
	Vector3 Pos;
	float Scale;


	SendPlayerData()
	{
		Header.m_Type = SST_PLAYER_DATA;
	}
};

class JEONG_DLL DeleteEatObject
{
public:
	Header Header;
	size_t Index;

	DeleteEatObject()
	{
		Header.m_Type = SST_DELETE_EAT_OBJECT;
	}
};

JEONG_END