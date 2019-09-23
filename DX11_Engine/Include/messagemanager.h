#pragma once

JEONG_BEGIN

class ReadMemoryStream;
class JEONG_DLL MessageManager
{
public:
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

private:
	void ClientMessageProcess();
	void ClientSend(IO_Data* Data);

	//Ŭ���̾�Ʈ ���������� �޼��������� �����ϴ� �Լ�.
	bool CreateMainPlayer(size_t ClientID, ReadMemoryStream& Reader);
	bool CreateOneOtherPlayer(size_t ClientID, ReadMemoryStream& Reader);
	bool CreateOtherPlayer(int ClientID, ReadMemoryStream& Reader);

	SEVER_DATA_TYPE ReadHeader(char* Buffer);

	void InitIOData(SocketInfo* Info);

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
