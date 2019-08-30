#pragma once

JEONG_BEGIN

class JEONG_DLL MessageManager
{
public:
	bool SendNewPlayerMsg(SocketInfo* Socket);
	bool SendOtharPlayerMsg(SocketInfo* Socket);

	bool ClientMessageProcess(SocketInfo * Socket, IO_Data * Data);
	bool SeverMesageProcess(SocketInfo * Socket, IO_Data * Data);

	//Ŭ����Լ�
	void SetScene(Scene* scene) { m_CurScene = scene; };
	Scene* GetScene() const { return m_CurScene; }
	void SetLayer(Layer* layer) { m_CurLayer = layer; }
	Layer* GetLayer() { return m_CurLayer; }

	void ClientInit();

private:
	SEVER_DATA_TYPE IOCPSeverRecvMsg(SocketInfo* Socket, IO_Data* Data);
	bool IOCPServerSend(SocketInfo* Socket, IO_Data* Data);
	SEVER_DATA_TYPE ClientRecvMsg();

	//Ŭ���̾�Ʈ ���������� �޼��������� �����ϴ� �Լ�.
	bool CreateMainPlayer();
	bool CreateOtherPlayer();

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
