#pragma once
JEONG_BEGIN

class JEONG_DLL DataManager
{
public:
	void PushClient(SocketInfo* Socket);
	void DeleteSocket(SocketInfo* Socket);
	SocketInfo* FindSocket(size_t ClientID);
	vector<SocketInfo*>* GetClientList() {	return &m_vecClient; }

public:
	static size_t m_ClientCount;

private:
	list<SocketInfo*> m_ClientList;
	vector<SocketInfo*> m_vecClient;

public:
	CLASS_IN_SINGLE(DataManager)
};

JEONG_END