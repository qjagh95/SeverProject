#pragma once
#include "WriteMemoryStream.h"

JEONG_BEGIN

struct SocketInfo
{
	SOCKET m_Socket;
	SOCKADDR_IN m_ClientInfo;
	size_t m_CliendID = 0;
};

struct IO_Data
{
	OVERLAPPED m_Overlapped;
	WSABUF m_WsaBuf;
	WriteMemoryStream m_Stream;

	IO_Data()
	{
		ZeroMemory(&m_Overlapped, sizeof(OVERLAPPED));
		m_WsaBuf.len = 0;
	}

	void WriteBuffer(const void* Buffer, size_t size)
	{
		m_Stream.Write(Buffer, size);
		CopyBuffer(size);
	}

	template<typename T>
	void WriteBuffer(const void* Buffer)
	{
		m_Stream.Write(Buffer, sizeof(T));
		CopyBuffer(sizeof(T));
	}

	template<typename T>
	void WriteHeader()
	{
		T Header;
		m_Stream.Write(&Header.m_Type, sizeof(4));
		CopyBuffer(4);
	}

	void HeaderErase()
	{
		m_Stream.HeaderErase();
	}

	void ClearBuffer()
	{
		m_Stream.BufferClear();
		ZeroMemory(m_WsaBuf.buf, m_Stream.GetSize());
		m_WsaBuf.len = 0;
	}

private:
	void CopyBuffer(size_t size)
	{
		m_WsaBuf.buf = m_Stream.GetBuffer();
		m_WsaBuf.len = static_cast<ULONG>(size);
	}
};

struct PlayerInfo
{
	Vector4* m_Color;
	Vector3* m_Pos;
	float* m_Scale;
};

class JEONG_DLL Header
{
public:
	SEVER_DATA_TYPE m_Type;

public:
	Header();
	~Header();
};

//밑으로는 보내거나 받는 타입을 명확하게 하기위해 메세지 클래스화
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


class JEONG_DLL ClientDieMessage : public Header
{
public:
	ClientDieMessage()
	{
		m_Type = SST_CLIENT_DIE;
	}
};

JEONG_END