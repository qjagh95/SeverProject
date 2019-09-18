#pragma once

JEONG_BEGIN

class ReadMemoryStream;
class JEONG_DLL MessageManager
{
public:
	//������ Ŭ��� �����÷��̾� ���� �޼��� 
	bool Sever_SendNewPlayerMsg(SocketInfo* Socket);

	//���� ������ Ŭ�� OT���͸޼���
	bool Sever_SendConnectClientNewOtherPlayer(SocketInfo* NewSocket);

	//���� �޼�������
	bool SeverMesageProcess(SocketInfo * Socket, IO_Data * Data);

	//Ŭ���̾�Ʈ Dieó��
	void Sever_DieClient(SocketInfo* Socket, IO_Data* Data);

	//���� �������� Ŭ���̾�Ʈ���� OT���Ÿ��(��������)
	void Sever_SendDeleteOT(SocketInfo * Socket);
	
	//Pos����
	void Sever_UpdatePos(SocketInfo * Socket, IO_Data* Data);

	//Scale����
	void Sever_UpdateScale(SocketInfo * Socket, IO_Data* Data);

	//�÷��̾� ������ ������
	void Sever_SendPlayerPos(SocketInfo * Socket, const Vector3& Pos);
	void Sever_SendPlayerScale(SocketInfo * Socket, float Scale);

	//Ŭ�� ������ ������ �Լ�
	void Client_ClientDie();
	void Client_SendPlayerPos(const Vector3& Pos);
	void Client_SendPlayerScale(float Scale);
	void OtherPlayerDie(ReadMemoryStream& Reader, size_t DeleteID);
	void Client_UpdateOTPos(ReadMemoryStream& Reader, size_t ID);
	void Client_UpdateOTScale(ReadMemoryStream& Reader, size_t ID);

	//Ŭ����Լ�
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

	//Ŭ���̾�Ʈ ���������� �޼��������� �����ϴ� �Լ�.
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
