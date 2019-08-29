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

//�����δ� �����ų� �޴� Ÿ���� ��Ȯ�ϰ� �ϱ����� �޼��� Ŭ����ȭ
class JEONG_DLL CreateMainPlayerMessage : public Header
{
public:
	CreateMainPlayerMessage()
	{
		m_Type = SST_CREATE_PLAYER;
	}
};

class JEONG_DLL CreateOtherPlayerMessage : public Header
{
public:
	CreateOtherPlayerMessage()
	{
		m_Type = SST_CREATE_OTHER_PLAYER;
	}
};

class JEONG_DLL CreateEatObjectMessage : public Header
{
public:
	CreateEatObjectMessage()
	{
		m_Type = SST_CREATE_EAT_OBJECT;
	}
};

class JEONG_DLL SendPlayerData : public Header
{
public:
	SendPlayerData()
	{
		m_Type = SST_PLAYER_DATA;
	}
};

class JEONG_DLL DeleteEatObject : public Header
{
public:
	DeleteEatObject()
	{
		m_Type = SST_DELETE_EAT_OBJECT;
	}
};

JEONG_END