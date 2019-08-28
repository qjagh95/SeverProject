#pragma once

JEONG_BEGIN

class JEONG_DLL MessageManager
{
public:
	void SendNewPlayerMsg(SocketInfo* Socket);
	void ClientMessageProcess(SocketInfo * Socket, IO_Data * Data);
	void SeverMesageProcess(SocketInfo * Socket, IO_Data * Data);
	void SetScene(Scene* scene) { m_CurScene = scene; };
	Scene* GetScene() const { return m_CurScene; }
	SEVER_DATA_TYPE RecvMsg(SocketInfo* Socket, IO_Data* Data);

private:
	void Send(SocketInfo* Socket, IO_Data* Data);
	void ReadHeader(char* Buffer);

private:
	SEVER_DATA_TYPE m_State;
	Scene* m_CurScene;

private:
	CLASS_IN_SINGLE(MessageManager)
};

JEONG_END
