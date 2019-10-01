#include "ClientHeader.h"
#include "ConnectSever.h"
#include <Core.h>

SINGLETON_VAR_INIT(ConnectSever)

ConnectSever::ConnectSever()
{
	m_ID = 0;
}

ConnectSever::~ConnectSever()
{
	WSACloseEvent(m_EventHandle);

	closesocket(m_Info.m_Socket);
	WSACleanup();
}

bool ConnectSever::Init()
{
	if (Core::Get()->m_ProjectType == MY_IS_CLIENT)
	{
		WSADATA Temp;
		WSAStartup(MAKEWORD(2, 2), &Temp);

		m_Info.m_Socket = socket(AF_INET, SOCK_STREAM, 0);

		string IPAddress = "192.168.219.102";

		m_Info.m_ClientInfo.sin_family = AF_INET;
		m_Info.m_ClientInfo.sin_port = htons(static_cast<uint16_t>(PORT));
		inet_pton(AF_INET, IPAddress.c_str(), &m_Info.m_ClientInfo.sin_addr);

		m_EventHandle = WSACreateEvent();

		if (WSAEventSelect(m_Info.m_Socket, m_EventHandle, FD_READ | FD_WRITE) != 0)
			assert(false);

		//소켓 옵션을 가져온다
		int optVar;
		int Len = sizeof(optVar);
		::getsockopt(m_Info.m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&optVar, &Len);

		//소켓 수신버퍼크기를 늘린다.
		optVar = 2048;
		::setsockopt(m_Info.m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&optVar, sizeof(optVar));

		Connect();
	}

	return true;
}

void ConnectSever::CloseSocket()
{
	closesocket(m_Info.m_Socket);
	WSACleanup();
}

void ConnectSever::Connect()
{
	connect(m_Info.m_Socket, reinterpret_cast<sockaddr*>(&m_Info.m_ClientInfo), sizeof(sockaddr_in));
}