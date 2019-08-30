#pragma once

JEONG_BEGIN

class JEONG_DLL MessageManager
{
public:
	bool SendNewPlayerMsg(SocketInfo* Socket);
	bool SendOtharPlayerMsg(SocketInfo* Socket);

	bool SeverMesageProcess(SocketInfo * Socket, IO_Data * Data);

	//클라용함수
	void SetScene(Scene* scene) { m_CurScene = scene; };
	Scene* GetScene() const { return m_CurScene; }
	void SetLayer(Layer* layer) { m_CurLayer = layer; }
	Layer* GetLayer() { return m_CurLayer; }

	void ClientInit();

private:
	SEVER_DATA_TYPE IOCPSeverRecvMsg(SocketInfo* Socket, IO_Data* Data);
	bool IOCPServerSend(SocketInfo* Socket, IO_Data* Data);
	void ClientMessageProcess();
	void ClientSend(IO_Data* Data);

	//클라이언트 실질적으로 메세지에따라 실행하는 함수.
	bool CreateMainPlayer();
	bool CreateOtherPlayer();

	SEVER_DATA_TYPE ReadHeader(char* Buffer);

private:
	SEVER_DATA_TYPE m_State;
	Scene* m_CurScene;
	Layer* m_CurLayer;
	thread m_Thread;
	mutex m_Mutex;
	IO_Data* m_ReadBuffer;
	IO_Data* m_WriteBuffer;

private:
	CLASS_IN_SINGLE(MessageManager)
};

JEONG_END
