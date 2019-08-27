#pragma once
JEONG_USING

class ConnectSever
{
public:
	bool Init();
	
	SOCKET GetSocket() const { return m_Socket; }
	sockaddr_in GetAddressInfo() const { return m_AddrInfo; }

private:
	void Connect();

private:
	SOCKET m_Socket;
	sockaddr_in m_AddrInfo;

public:
	CLASS_IN_SINGLE(ConnectSever)
};

