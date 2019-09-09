#pragma once

JEONG_BEGIN

class ReadMemoryStream;
class JEONG_DLL MessageManager
{
public:
	//������ Ŭ��� �����÷��̾� ���� �޼��� 
	bool Sever_SendNewPlayerMsg(SocketInfo* Socket);

	//���Ӱ� ������ Ŭ�� ���� ������ Ŭ�󰹼���ŭ OT���� �޼���
	bool Sever_NewClientCreateOtherPlayer(SocketInfo* Socket);

	//���� ������ Ŭ�� OT���͸޼���
	bool Sever_SendConnectClientNewOtherPlayer(SocketInfo* NewSocket);

	//���� �޼�������
	bool SeverMesageProcess(SocketInfo * Socket, IO_Data * Data);

	//Ŭ���̾�Ʈ Dieó��
	void Sever_DieClient(SocketInfo* Socket);

	//Ŭ�� ������ ������ �Լ�
	void Client_ClientDie();

	//Ŭ����Լ�
	void SetScene(Scene* scene) { m_CurScene = scene; };
	Scene* GetScene() const { return m_CurScene; }
	void SetLayer(Layer* layer) { m_CurLayer = layer; }
	Layer* GetLayer() { return m_CurLayer; }

	void ClientInit();
	SEVER_DATA_TYPE IOCPSeverRecvMsg(SocketInfo* Socket, IO_Data* Data);

private:
	bool IOCPServerSend(SocketInfo* Socket, IO_Data* Data);
	bool IOCPSeverSendALL(SocketInfo* SameSocket, IO_Data* Data);
	void ClientMessageProcess();
	void ClientSend(IO_Data* Data);

	//Ŭ���̾�Ʈ ���������� �޼��������� �����ϴ� �Լ�.
	bool CreateMainPlayer(size_t ClientID, ReadMemoryStream& Reader);
	bool CreateOneOtherPlayer(size_t ClientID, ReadMemoryStream& Reader);
	bool CreateOtherPlayer(size_t ClientID, ReadMemoryStream& Reader);

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
