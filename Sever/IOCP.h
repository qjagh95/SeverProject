#pragma once

JEONG_USING

class PlayerInfo;
class IOCP
{
public:
	bool Init();
	void Run();
		
	void ThreadFunc();

private:
	void DeleteSocket(SocketInfo* Socket);
	void SetSocket();
	void SendAllClient(SocketInfo* Socket, char* Data);
	void MessageProcess(SocketInfo* Socket, IO_Data* Data);

private:
	HANDLE m_CompletionPort;
	SocketInfo m_SeverSocket;
	vector<thread*> m_vecThread;
	list<SocketInfo*> m_ClientList;
	vector<PlayerInfo*> m_vecPlayerInfo;

public:
	IOCP();
	~IOCP();		
};

