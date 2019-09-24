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
	int	m_Mode;

	IO_Data()
	{
		ZeroMemory(&m_Overlapped, sizeof(OVERLAPPED));
		m_WsaBuf.len = 0;
		m_Mode = -1;
	}

	void WriteBuffer(const void* Buffer, size_t size)
	{
		m_Stream.Write(Buffer, size);
		CopyBuffer();
	}

	size_t GetSize() const { return m_Stream.GetSize(); }

	template<typename T>
	void WriteBuffer(const void* Buffer)
	{
		m_Stream.Write(Buffer, sizeof(T));
		CopyBuffer();
	}

	template<typename T>
	void WriteHeader()
	{
		T Header;
		m_Stream.Write(&Header.m_Type, 4);
		CopyBuffer();
	}

	void HeaderErase()
	{
		m_Stream.HeaderErase();
	}

	void PullBuffer(size_t Size)
	{
		if (m_Stream.GetSize() == 0)
			return;

		m_Stream.PullBuffer(Size);
	}

	template<typename T>
	void PullBuffer()
	{
		if (m_Stream.GetSize() == 0)
			return;

		m_Stream.PullBuffer(sizeof(T));
	}

	void ClearBuffer()
	{
		m_Stream.BufferClear();
		ZeroMemory(m_WsaBuf.buf, m_Stream.GetSize());
		m_WsaBuf.len = 0;
	}

	char* GetBuffer()
	{
		return m_Stream.GetBuffer();
	}

	SEVER_DATA_TYPE ReadHeader()
	{
		SEVER_DATA_TYPE Temp = SST_NONE;

		if (m_WsaBuf.len == 0)
			return Temp;

		memcpy(&Temp, m_WsaBuf.buf, 4);

		return Temp;
	}

	void CopyBuffer()
	{
		if (m_Stream.GetSize() == 0)
		{
			m_Stream.Write(m_WsaBuf.buf, m_WsaBuf.len);
			m_WsaBuf.len = static_cast<ULONG>(m_Stream.GetSize());
			m_WsaBuf.buf = m_Stream.GetBuffer();
		}
		else
		{
			m_WsaBuf.buf = m_Stream.GetBuffer();
			m_WsaBuf.len = static_cast<ULONG>(m_Stream.GetSize());
		}
	}
};

struct PlayerInfo
{
	size_t m_ClientID;
	Vector4 m_Color;
	Vector3 m_Pos;
	float m_Scale;
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

class JEONG_DLL CreateConnectClientCreateOtherPlayer : public Header
{
public:
	CreateConnectClientCreateOtherPlayer()
	{
		m_Type = SST_CONNECT_CLIENT_CREATE_OTHER_PLAYER;
	}
};

class JEONG_DLL OtherPlayerDelete : public Header
{
public:
	OtherPlayerDelete()
	{
		m_Type = SST_OTHER_PLAYER_DELETE;
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

class JEONG_DLL PlayerPosMessage : public Header
{
public:
	PlayerPosMessage()
	{
		m_Type = SST_PLAYER_POS;
	}
};

class JEONG_DLL PlayerScaleMessage : public Header
{
public:
	PlayerScaleMessage()
	{
		m_Type = SST_PLAYER_POS;
	}
};

JEONG_END