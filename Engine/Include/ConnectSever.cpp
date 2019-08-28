#include "stdafx.h"
#include "ConnectSever.h"

JEONG_USING
SINGLETON_VAR_INIT(ConnectSever)

ConnectSever::ConnectSever()
{
}

ConnectSever::~ConnectSever()
{
}

bool ConnectSever::Init(PROJECT_TYPE Type)
{
	if (Type == MY_IS_CLIENT)
	{
		m_Info.m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULLPTR, 0, WSA_FLAG_OVERLAPPED);

		WSADATA Temp;
		WSAStartup(MAKEWORD(2, 2), &Temp);

		string IPAddress = "192.168.0.2";

		m_Info.m_ClientInfo.sin_family = AF_INET;
		m_Info.m_ClientInfo.sin_port = PORT;
		inet_pton(AF_INET, IPAddress.c_str(), &m_Info.m_ClientInfo.sin_addr);

		Connect();
	}

	return true;
}

void ConnectSever::Connect()
{
	connect(m_Info.m_Socket, reinterpret_cast<sockaddr*>(&m_Info.m_ClientInfo), sizeof(sockaddr_in));
}

