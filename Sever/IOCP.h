#pragma once

JEONG_USING

class IOCP
{
public:
	bool Init();
	void Run();
		
	void ThreadFunc();

private:
	void SetSocket();

	void IOCPSeverSend(SocketInfo * Socket, IO_Data * Data);

	//������ Ŭ��� �����÷��̾� ���� �޼��� 
	void Sever_SendNewPlayerMsg(SocketInfo * Socket);

	//���� ������ Ŭ�� OT���͸޼���
	void Sever_SendConnectClientNewOtherPlayer(SocketInfo* NewSocket);

	//���� �޼�������
	void _stdcall SeverMesageProcess(SocketInfo * Socket, char * Data, size_t BufferSize);

	//Ŭ���̾�Ʈ Dieó��
	void Sever_DieClient(SocketInfo* Socket);

	//���� �������� Ŭ���̾�Ʈ���� OT���Ÿ��(��������)
	void Sever_SendDeleteOT(SocketInfo * Socket);

	//Pos����
	void Sever_UpdatePos(SocketInfo * Socket, ReadMemoryStream& Reader);

	//Scale����
	void Sever_UpdateScale(SocketInfo * Socket, ReadMemoryStream& Reader);

	//�÷��̾� ������ ������
	void Sever_SendPlayerPos(SocketInfo * Socket, const Vector3& Pos);
	void Sever_SendPlayerScale(SocketInfo * Socket, float Scale);

	void RecvInitIOData(SocketInfo* Info);
	SEVER_DATA_TYPE ReadHeader(char* Buffer);
	SEVER_DATA_TYPE IOCPSeverRecvMsg(SocketInfo* Socket, IO_Data* Data);

private:
	HANDLE m_CompletionPort;
	SocketInfo m_SeverSocket;
	vector<thread*> m_vecThread;
	SocketInfo* m_SocketInfo;
	mutex m_Mutex;
	SEVER_DATA_TYPE m_State;

public:
	IOCP();
	~IOCP();		
};

