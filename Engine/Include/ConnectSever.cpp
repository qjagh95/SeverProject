#include "stdafx.h"
#include "ConnectSever.h"
#include "Core.h"

JEONG_USING
SINGLETON_VAR_INIT(ConnectSever)

ConnectSever::ConnectSever()
{
}

ConnectSever::~ConnectSever()
{
	WSACloseEvent(m_EventHandle);
}

bool ConnectSever::Init()
{
	if (Core::Get()->m_ProjectType == MY_IS_CLIENT)
	{
		WSADATA Temp;
		WSAStartup(MAKEWORD(2, 2), &Temp);

		m_Info.m_Socket = socket(AF_INET, SOCK_STREAM, 0);

		string IPAddress = "192.168.1.164";

		m_Info.m_ClientInfo.sin_family = AF_INET;
		m_Info.m_ClientInfo.sin_port = htons(static_cast<uint16_t>(PORT));
		inet_pton(AF_INET, IPAddress.c_str(), &m_Info.m_ClientInfo.sin_addr);

		m_EventHandle = WSACreateEvent();

		if (WSAEventSelect(m_Info.m_Socket, m_EventHandle, FD_READ | FD_WRITE) != 0)
			assert(false);

		Connect();
	}

	return true;
}

void ConnectSever::Connect()
{
	connect(m_Info.m_Socket, reinterpret_cast<sockaddr*>(&m_Info.m_ClientInfo), sizeof(sockaddr_in));
}
