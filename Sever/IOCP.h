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

	//서버가 클라로 메인플레이어 생성 메세지 
	void Sever_SendNewPlayerMsg(SocketInfo * Socket);
	//현재 접속한 클라에 OT생셩메세지
	void Sever_SendConnectClientNewOtherPlayer(SocketInfo* NewSocket);
	//서버 메세지루프
	void SeverMesageProcess(SocketInfo * Socket, char * Data, size_t BufferSize);
	//클라이언트 Die처리
	void Sever_DieClient(SocketInfo* Socket);
	//현재 접속중인 클라이언트에게 OT제거명령(접속종료)
	void Sever_SendDeleteOT(SocketInfo * Socket);
	//Pos갱신
	void Sever_UpdatePos(SocketInfo * Socket, ReadMemoryStream& Reader);
	//Scale갱신
	void Sever_UpdateScale(SocketInfo * Socket, ReadMemoryStream& Reader);
	//EatObject삭제
	void Sever_DeleteEatObject(SocketInfo * Socket, ReadMemoryStream& Reader);
	//플레이어 데이터 보내기
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

