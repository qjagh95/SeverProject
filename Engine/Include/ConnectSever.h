#pragma once

JEONG_BEGIN

class JEONG_DLL ConnectSever
{
public:
	bool Init(PROJECT_TYPE Type = MY_IS_CLIENT);
	
	SOCKET* GetSocket() { return &m_Info.m_Socket; }
	sockaddr_in* GetAddressInfo() { return &m_Info.m_ClientInfo; }
	SocketInfo* GetSocketInfo() { return &m_Info; }

private:
	void Connect();

private:
	SocketInfo m_Info;

public:
	CLASS_IN_SINGLE(ConnectSever)
};

JEONG_END