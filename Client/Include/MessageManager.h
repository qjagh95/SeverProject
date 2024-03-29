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
	//클라가 서버로 보내는 함수
	void Client_ClientDie();
	void Client_SendPlayerPos(const Vector3& Pos, const Vector3& CameraPos);
	void Client_SendDieEatting(int DeleteEatID);

	void Client_UpdateOTPos(ReadMemoryStream& Reader, int ID);
	void Client_UpdateOTScale(ReadMemoryStream& Reader, int ID);
	void OtherPlayerDie(size_t DeleteID);

	//클라용함수
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

	//클라이언트 실질적으로 메세지에따라 실행하는 함수.
	bool CreateMainPlayer(int ClientID, ReadMemoryStream& Reader);
	bool CreateOneOtherPlayer(int ClientID, ReadMemoryStream& Reader);
	bool CreateOtherPlayer(int ClientID, ReadMemoryStream& Reader);
	void CreateEat(int ClientID, ReadMemoryStream& Reader);
	void CreateEat(ReadMemoryStream& Reader);
	void DeleteEatOfScale(int ID, ReadMemoryStream& Reader);

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
