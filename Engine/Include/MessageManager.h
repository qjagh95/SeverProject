#pragma once

JEONG_BEGIN

class ReadMemoryStream;
class JEONG_DLL MessageManager
{
public:
	//서버가 클라로 메인플레이어 생성 메세지 
	bool Sever_SendNewPlayerMsg(SocketInfo* Socket);

	//현재 접속한 클라에 OT생셩메세지
	bool Sever_SendConnectClientNewOtherPlayer(SocketInfo* NewSocket);

	//서버 메세지루프
	bool SeverMesageProcess(SocketInfo * Socket, IO_Data * Data);

	//클라이언트 Die처리
	void Sever_DieClient(SocketInfo* Socket, IO_Data* Data);

	//현재 접속중인 클라이언트에게 OT제거명령(접속종료)
	void Sever_SendDeleteOT(SocketInfo * Socket);
	
	//Pos갱신
	void Sever_UpdatePos(SocketInfo * Socket, IO_Data* Data);

	//Scale갱신
	void Sever_UpdateScale(SocketInfo * Socket, IO_Data* Data);

	//플레이어 데이터 보내기
	void Sever_SendPlayerPos(SocketInfo * Socket, const Vector3& Pos);
	void Sever_SendPlayerScale(SocketInfo * Socket, float Scale);

	//클라가 서버로 보내는 함수
	void Client_ClientDie();
	void Client_SendPlayerPos(const Vector3& Pos);
	void Client_SendPlayerScale(float Scale);
	void OtherPlayerDie(ReadMemoryStream& Reader, size_t DeleteID);
	void Client_UpdateOTPos(ReadMemoryStream& Reader, size_t ID);
	void Client_UpdateOTScale(ReadMemoryStream& Reader, size_t ID);

	//클라용함수
	void SetScene(Scene* scene) { m_CurScene = scene; };
	Scene* GetScene() const { return m_CurScene; }
	void SetLayer(Layer* layer) { m_CurLayer = layer; }
	Layer* GetLayer() { return m_CurLayer; }

	void ClientInit();
	SEVER_DATA_TYPE IOCPSeverRecvMsg(SocketInfo* Socket, IO_Data* Data);

private:
	bool IOCPServerSend(SocketInfo* Socket, IO_Data* Data);
	void ClientMessageProcess();
	void ClientSend(IO_Data* Data);

	//클라이언트 실질적으로 메세지에따라 실행하는 함수.
	bool CreateMainPlayer(size_t ClientID, ReadMemoryStream& Reader);
	bool CreateOneOtherPlayer(size_t ClientID, ReadMemoryStream& Reader);
	bool CreateOtherPlayer(int ClientID, ReadMemoryStream& Reader);

	SEVER_DATA_TYPE ReadHeader(char* Buffer);

private:
	SEVER_DATA_TYPE m_State;
	Scene* m_CurScene;
	Layer* m_CurLayer;
	thread m_Thread;
	mutex m_Mutex;

private:
	CLASS_IN_SINGLE(MessageManager)
};

JEONG_END
