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
	void SetSocket();

	void IOCPSeverSend(SocketInfo * Socket, IO_Data * Data);

private:
	HANDLE m_CompletionPort;
	SocketInfo m_SeverSocket;
	vector<thread*> m_vecThread;
	IO_Data* m_IOData;
	SocketInfo* m_SocketInfo;
	mutex m_Mutex;

public:
	IOCP();
	~IOCP();		
};

