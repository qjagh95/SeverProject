#pragma once

JEONG_BEGIN

class JEONG_DLL MessageManager
{
public:
	bool SendNewPlayerMsg(SocketInfo* Socket);
	bool SendOtharPlayerMsg(SocketInfo* Socket);
	bool ClientMessageProcess(SocketInfo * Socket, IO_Data * Data);
	bool SeverMesageProcess(SocketInfo * Socket, IO_Data * Data);

	//클라용함수
	void SetScene(Scene* scene) { m_CurScene = scene; };
	Scene* GetScene() const { return m_CurScene; }
	void SetLayer(Layer* layer) { m_CurLayer = layer; }
	Layer* GetLayer() { return m_CurLayer; }

private:
	SEVER_DATA_TYPE RecvMsg(SocketInfo* Socket, IO_Data* Data);
	bool Send(SocketInfo* Socket, IO_Data* Data);
	void ReadHeader(char* Buffer);

	bool CreateMainPlayer();
	bool CreateOtherPlayer();
	bool CreateEattingObject();

private:
	SEVER_DATA_TYPE m_State;
	Scene* m_CurScene;
	Layer* m_CurLayer;

private:
	CLASS_IN_SINGLE(MessageManager)
};

JEONG_END
