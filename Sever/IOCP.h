#pragma once
class IOCP
{
public:
	bool Init();
	void Run();
		
	void ThreadFunc();

private:
	void DeleteSocket(SocketInfo* Socket);
	void SetSocket();
	void MessageProcess();

private:
	HANDLE m_CompletionPort;
	SocketInfo m_SeverSocket;
	vector<thread*> m_vecThread;
	list<SocketInfo*> m_ClientList;

public:
	IOCP();
	~IOCP();		
};

