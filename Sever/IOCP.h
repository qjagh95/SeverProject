#pragma once

JEONG_USING

#include <ReadMemoryStream.h>
#include <WriteMemoryStream.h>

class IOCP
{
public:
	bool Init();
	void Run();
		
	void ThreadFunc();

private:
	void IOCPSeverSend(SocketInfo * Socket, IO_Data * Data);

	//������ Ŭ��� �����÷��̾� ���� �޼��� 
	void Sever_SendNewPlayerMsg(SocketInfo * Socket);
	//���� ������ Ŭ�� OT���͸޼���
	void Sever_SendConnectClientNewOtherPlayer(SocketInfo* NewSocket);
	//���� �޼�������
	void SeverMesageProcess(SocketInfo * Socket, char * Data, size_t BufferSize);
	//Ŭ���̾�Ʈ Dieó��
	void Sever_DieClient(SocketInfo* Socket);
	//���� �������� Ŭ���̾�Ʈ���� OT���Ÿ��(��������)
	void Sever_SendDeleteOT(SocketInfo * Socket);
	//Pos����
	void Sever_UpdatePos(SocketInfo * Socket, ReadMemoryStream& Reader);
	//Scale����
	void Sever_UpdateScale(SocketInfo * Socket, ReadMemoryStream& Reader);
	//EatObject����
	void Sever_DeleteEatObject(SocketInfo * Socket, ReadMemoryStream& Reader);
	//�÷��̾� ������ ������
	void Sever_SendPlayerPos(SocketInfo * Socket, const Vector3& CameraPos, int UpdateVecSize);
	void Sever_SendPlayerScale(SocketInfo * Socket, float Scale);

	void RecvInitIOData(SocketInfo* Info);
	SEVER_DATA_TYPE ReadHeader(char* Buffer);
	SEVER_DATA_TYPE IOCPSeverRecvMsg(SocketInfo* Socket, IO_Data* Data);

	void Sever_SendFirstSeeList(SocketInfo * Socket);

	void KeyFunc();

private:
	HANDLE m_CompletionPort;
	SocketInfo m_SeverSocket;
	vector<thread*> m_vecThread;
	SocketInfo* m_SocketInfo;
	mutex m_Mutex;
	SEVER_DATA_TYPE m_State;
	float m_TimeVar;
	float m_OneSecond;
	thread m_KeyThread;
	bool m_isCheck;

public:
	IOCP();
	~IOCP();		
};

