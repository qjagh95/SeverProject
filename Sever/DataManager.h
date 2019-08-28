#pragma once
JEONG_USING
class PlayerInfo;
class DataManager
{
public:
	void PushClient(SocketInfo* Socket);
	void PushPlayer(PlayerInfo* Object);
	void DeleteSocket(SocketInfo* Socket);
	void SendAllClient(SocketInfo* Socket, IO_Data* Data);
	void MessageProcess(SocketInfo* Socket, IO_Data* Data);
	SocketInfo* FindSocket(size_t ClientID);

private:


public:
	static size_t m_ClientCount;

private:
	list<SocketInfo*> m_ClientList;
	list<PlayerInfo*> m_vecPlayerInfo;
	vector<SocketInfo*> m_vecClient;
	SEVER_DATA_TYPE m_State;

public:
	CLASS_IN_SINGLE(DataManager)
};

