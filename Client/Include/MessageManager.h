#pragma once

#include <ReadMemoryStream.h>
#include <WriteMemoryStream.h>
#include "UserComponent/Player_Com.h"

JEONG_USING
class Player_Com;
class Stage_Com;
class MessageManager
{
public:
	//Ŭ�� ������ ������ �Լ�
	void Client_ClientDie();
	void Client_SendPlayerPos(const Vector3& Pos, const Vector3& CameraPos);
	void Client_SendPlayerScale(float Scale);
	void Client_UpdateOTPos(ReadMemoryStream& Reader, size_t ID);
	void Client_UpdateOTScale(ReadMemoryStream& Reader, size_t ID);
	void OtherPlayerDie(size_t DeleteID);

	//Ŭ����Լ�
	void SetScene(Scene* scene) { m_CurScene = scene; };
	Scene* GetScene() const { return m_CurScene; }
	void SetLayer(Layer* layer) { m_CurLayer = layer; }
	Layer* GetLayer() { return m_CurLayer; }

	void ClientInit();
	void SetStage(Stage_Com* CurStage) { m_CurStage = CurStage; }

private:
	void ClientMessageProcess();
	void ClientSend(IO_Data* Data);
	void ClearRecvBuffer();

	//Ŭ���̾�Ʈ ���������� �޼��������� �����ϴ� �Լ�.
	bool CreateMainPlayer(size_t ClientID, ReadMemoryStream& Reader);
	bool CreateOneOtherPlayer(size_t ClientID, ReadMemoryStream& Reader);
	bool CreateOtherPlayer(int ClientID, ReadMemoryStream& Reader);
	void CreateEat(size_t ClientID, ReadMemoryStream& Reader);
	void UpdateEat(size_t ClientID, ReadMemoryStream& Reader);

	SEVER_DATA_TYPE ReadHeader(char* Buffer);

private:
	SEVER_DATA_TYPE m_State;
	Scene* m_CurScene;
	Layer* m_CurLayer;
	thread m_Thread;
	mutex m_Mutex;
	Stage_Com* m_CurStage;

private:
	CLASS_IN_SINGLE(MessageManager)
};
