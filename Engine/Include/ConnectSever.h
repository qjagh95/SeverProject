#pragma once

JEONG_BEGIN

class JEONG_DLL ConnectSever
{
public:
	bool Init();
	
	SOCKET* GetSocket() { return &m_Info.m_Socket; }
	sockaddr_in* GetAddressInfo() { return &m_Info.m_ClientInfo; }
	SocketInfo* GetSocketInfo() { return &m_Info; }
	WSAEVENT GetEventHandle() { return m_EventHandle; }

private:
	void Connect();

private:
	SocketInfo m_Info;
	WSAEVENT m_EventHandle;
	//WSAEVENT == HANDLE
	
public:
	CLASS_IN_SINGLE(ConnectSever)
};

JEONG_END