#pragma once

#include <ReadMemoryStream.h>
#include <WriteMemoryStream.h>
#include "UserComponent/Player_Com.h"

JEONG_USING
class Player_Com;
class MessageManager
{
public:
	//클라가 서버로 보내는 함수
	void Client_ClientDie();
	void Client_SendPlayerPos(const Vector3& Pos);
	void Client_SendPlayerScale(float Scale);
	void Client_UpdateOTPos(ReadMemoryStream& Reader, size_t ID);
	void Client_UpdateOTScale(ReadMemoryStream& Reader, size_t ID);
	void OtherPlayerDie(size_t DeleteID);

	//클라용함수
	void SetScene(Scene* scene) { m_CurScene = scene; };
	Scene* GetScene() const { return m_CurScene; }
	void SetLayer(Layer* layer) { m_CurLayer = layer; }
	Layer* GetLayer() { return m_CurLayer; }

	void ClientInit();

private:
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
	float m_TimeVar;
	float m_Second;

private:
	CLASS_IN_SINGLE(MessageManager)
};
