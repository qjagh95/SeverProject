#include "ClientHeader.h"
#include "ConnectSever.h"

SINGLETON_VAR_INIT(ConnectSever)

ConnectSever::ConnectSever()
{
}

ConnectSever::~ConnectSever()
{
}

bool ConnectSever::Init()
{
	////소켓연결준비
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULLPTR, 0, WSA_FLAG_OVERLAPPED);

	WSADATA Temp;
	WSAStartup(MAKEWORD(2, 2), &Temp);

	string IPAddress = "192.168.0.2";

	m_AddrInfo.sin_family = AF_INET;
	m_AddrInfo.sin_port = PORT;
	inet_pton(AF_INET, IPAddress.c_str(), &m_AddrInfo.sin_addr);

	Connect();
	
	return true;
}

void ConnectSever::Connect()
{
	connect(m_Socket, reinterpret_cast<sockaddr*>(&m_AddrInfo), sizeof(m_AddrInfo));
}

