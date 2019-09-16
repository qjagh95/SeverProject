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
	void SetClientID(size_t ID) { m_ID = ID; }
	size_t GetClientID() const { return m_ID; }

private:
	void Connect();

private:
	SocketInfo m_Info;
	size_t m_ID;
	WSAEVENT m_EventHandle;
	//WSAEVENT == HANDLE
	
public:
	CLASS_IN_SINGLE(ConnectSever)
};

JEONG_END