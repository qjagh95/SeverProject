#include "ClientHeader.h"
#include <Core.h>
#include "MessageManager.h"
#include "DataManager.h"
#include "UserComponent/OtharPlayer_Com.h"
#include "ConnectSever.h"
#include "UserComponent/OTManager.h"

SINGLETON_VAR_INIT(MessageManager)

MessageManager::MessageManager()
{
	m_State = SST_NONE;
	m_CurLayer = NULLPTR;
	m_CurScene = NULLPTR;
}

MessageManager::~MessageManager()
{
	m_Thread.detach();
}
 
void MessageManager::ClientInit()
{
	m_Thread = thread(&MessageManager::ClientMessageProcess, this);
}

void MessageManager::Client_ClientDie()
{
	size_t MyID = ConnectSever::Get()->GetClientID();

	IO_Data IoData = {};
	IoData.WriteHeader<ClientDieMessage>();
	IoData.WriteBuffer<size_t>(&MyID);

	ClientSend(&IoData);
	ConnectSever::Get()->CloseSocket();
}

void MessageManager::Client_SendPlayerPos(const Vector3& Pos)
{
	size_t MyID = ConnectSever::Get()->GetClientID();

	IO_Data IoData = {};
	IoData.WriteHeader<PlayerPosMessage>();
	IoData.WriteBuffer<size_t>(&MyID);
	IoData.WriteBuffer<Vector3>(&Pos);

	ClientSend(&IoData);
}

void MessageManager::Client_SendPlayerScale(float Scale)
{
	size_t MyID = ConnectSever::Get()->GetClientID();

	IO_Data IoData = {};
	IoData.WriteHeader<PlayerScaleMessage>();
	IoData.WriteBuffer<size_t>(&MyID);
	IoData.WriteBuffer<float>(&Scale);

	ClientSend(&IoData);
}

void MessageManager::OtherPlayerDie(size_t DeleteID)
{
	OTManager::Get()->DeleteOT(DeleteID);
}

void MessageManager::Client_UpdateOTPos(ReadMemoryStream & Reader, size_t ID)
{
	auto getOT = OTManager::Get()->FindOT(ID);

	if (getOT == NULLPTR)
		return;

	//Pos받음
	Vector3 getPos = Reader.Read<Vector3>();

	getOT->GetTransform()->SetWorldPos(getPos);
}

void MessageManager::Client_UpdateOTScale(ReadMemoryStream & Reader, size_t ID)
{
	auto getOT = OTManager::Get()->FindOT(ID);

	if (getOT == NULLPTR)
	{
		cout << "Error! OT가 없습니다" << endl;
		TrueAssert(true);
		return;
	}

	float getScale = Reader.Read<float>();
	getOT->GetTransform()->SetWorldScale(getScale, getScale, 1.0f);
}

void MessageManager::ClientMessageProcess()
{
	WSAEVENT getEvent = ConnectSever::Get()->GetEventHandle();

	while (true)
	{
		if (m_CurScene == NULLPTR || m_CurLayer == NULLPTR)
			continue;

		int Event = WSAWaitForMultipleEvents(1, &getEvent, TRUE, WSA_INFINITE, FALSE);

		if (Event == WSA_WAIT_TIMEOUT || Event == WSA_WAIT_FAILED)
			continue;
		
		WSANETWORKEVENTS NetWorkEvent;
		SOCKET getSocket = *ConnectSever::Get()->GetSocket();
		WSAEnumNetworkEvents(getSocket, getEvent, &NetWorkEvent);

		if (NetWorkEvent.lNetworkEvents & FD_READ)
		{
			char Buffer[BUFFERSIZE] = { };
			recv(getSocket, Buffer, BUFFERSIZE, 0);

			ReadMemoryStream Reader = ReadMemoryStream(Buffer, BUFFERSIZE);
			m_State = Reader.Read<SEVER_DATA_TYPE>();
			size_t ClientID = Reader.Read<size_t>();

			switch (m_State)
			{
			case SST_PLAYER_POS:
				Client_UpdateOTPos(Reader, ClientID);
				break;
			case SST_PLAYER_SCALE:
				Client_UpdateOTScale(Reader, ClientID);
				break;
			case SST_OTHER_PLAYER_DELETE:
				OtherPlayerDie(ClientID);
				break;
			case SST_CREATE_EAT_OBJECT:
				break;
			case SST_CREATE_PLAYER:
				CreateMainPlayer(ClientID, Reader);
				break;
			case SST_CONNECT_CLIENT_CREATE_OTHER_PLAYER:
				CreateOneOtherPlayer(ClientID, Reader);
				break;
			case SST_DELETE_EAT_OBJECT:
				break;
			}

			ClearRecvBuffer();
		}
	}

	return;
}

void MessageManager::ClientSend(IO_Data * Data)
{
	auto getSocket = ConnectSever::Get()->GetSocketInfo();
	Data->CopyBuffer();
	
	int SendByte = send(getSocket->m_Socket, Data->GetBuffer(), static_cast<int>(Data->GetSize()), 0);

	if (SendByte == 0)
		cout << "Error : " << GetLastError() << endl;
}

void MessageManager::ClearRecvBuffer()
{
	unsigned long TempLong = 0;
	auto getSock = *ConnectSever::Get()->GetSocket();
	char TempBuf;
	ioctlsocket(getSock, FIONREAD, &TempLong);

	for (size_t i = 0; i < TempLong; i++)
		recv(getSock, &TempBuf, 1, 0);
}

SEVER_DATA_TYPE MessageManager::ReadHeader(char * Buffer)
{
	SEVER_DATA_TYPE HeaderType = SST_NONE;
	memcpy(&HeaderType, Buffer, sizeof(Header));

	return HeaderType;
}

bool MessageManager::CreateMainPlayer(size_t ClientID, ReadMemoryStream& Reader)
{
	Vector4 Color = Reader.Read<Vector4>();
	Vector3 Pos = Reader.Read<Vector3>();
	float Scale = Reader.Read<float>();

	GameObject* newPlayerObj = GameObject::CreateObject("Player", m_CurLayer);
	Player_Com* newPlayer = newPlayerObj->AddComponent<Player_Com>("Player");
	newPlayer->SetRGB(Color.x, Color.y, Color.z);
	newPlayer->SetScale(Scale);
	newPlayer->GetTransform()->SetWorldPos(Pos);

	m_CurScene->GetMainCamera()->SetTarget(newPlayerObj);

	ConnectSever::Get()->SetClientID(ClientID);

	SAFE_RELEASE(newPlayerObj);
	SAFE_RELEASE(newPlayer);

	int ClientCount = Reader.Read<int>();
	if (ClientCount == 1)
		return true;

	CreateOtherPlayer(ClientCount, Reader);

	m_State = SST_NONE;
	return true;
}

bool MessageManager::CreateOneOtherPlayer(size_t ClientID, ReadMemoryStream& Reader)
{
	Vector4 Color = Reader.Read<Vector4>();
	Vector3 Pos = Reader.Read<Vector3>();
	float Scale = Reader.Read<float>();

	GameObject* newOtherPlayerObj = GameObject::CreateObject("OtherPlayer", m_CurLayer);
	OtharPlayer_Com* newOther = newOtherPlayerObj->AddComponent<OtharPlayer_Com>("OtherPlayer");
	newOther->GetTransform()->SetWorldScale(Scale, Scale, 1.0f);
	newOther->GetTransform()->SetWorldPos(Pos);
	newOther->SetRGB(Color.x, Color.y, Color.z);

	OTManager::Get()->InsertOT(ClientID, newOther);

	SAFE_RELEASE(newOtherPlayerObj);
	SAFE_RELEASE(newOther);

	return true;
}

bool MessageManager::CreateOtherPlayer(int ClientID, ReadMemoryStream & Reader)
{
	//ClientID가 클라이언트 갯수를 보낼것임 (내꺼빼고).
	for (int i = 0; i < ClientID - 1; i++)
	{
		Vector4 Color = Reader.Read<Vector4>();
		Vector3 Pos = Reader.Read<Vector3>();
		float Scale = Reader.Read<float>();
		size_t ID = Reader.Read<size_t>();

		GameObject* newOtherPlayerObj = GameObject::CreateObject("OtherPlayer", m_CurLayer);
		OtharPlayer_Com* newOther = newOtherPlayerObj->AddComponent<OtharPlayer_Com>("OtherPlayer");
		newOther->GetTransform()->SetWorldScale(Scale, Scale, 1.0f);
		newOther->GetTransform()->SetWorldPos(Pos);
		newOther->SetRGB(Color.x, Color.y, Color.z);

		OTManager::Get()->InsertOT(ID, newOther);

		SAFE_RELEASE(newOtherPlayerObj);
		SAFE_RELEASE(newOther);
	}

	return false;
}
