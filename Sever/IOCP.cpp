#include "pch.h"
#include "IOCP.h"

#include "DataManager.h"
#include <WriteMemoryStream.h>
#include <ReadMemoryStream.h>
#include <Core.h>
#include <Timer.h>

JEONG_USING

IOCP::IOCP()
{
	m_State = SST_NONE;
	m_SeverSocket.m_CliendID = DataManager::m_ClientCount;
	TimeManager::Get()->Init();

	m_TimeVar = 0.0f;
	m_OneSecond = 1.0f;
}

IOCP::~IOCP()
{
	for (size_t i = 0; i < m_vecThread.size(); i++)
		m_vecThread[i]->join();

	Safe_Delete_VecList(m_vecThread);
	closesocket(m_SeverSocket.m_Socket);
	CloseHandle(m_CompletionPort);
	WSACleanup();

	DataManager::Get()->CloseAll();
}

bool IOCP::Init()
{
	WSAData Buf;
	SYSTEM_INFO Info;

	if (WSAStartup(MAKEWORD(2, 2), &Buf) != 0)
		assert(false);

	//CompleationPort 생성
	m_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULLPTR, 0, 0);
	GetSystemInfo(&Info);

	//CPU갯수 만큼 스레드 생성
	for (size_t i = 0; i < Info.dwNumberOfProcessors * 2; i++)
		m_vecThread.push_back(new thread(&IOCP::ThreadFunc, this));

	string Address = "192.168.1.172";

	m_SeverSocket.m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (m_SeverSocket.m_Socket == INVALID_SOCKET)
		assert(false);

	// 바인딩할 소켓 주소정보
	m_SeverSocket.m_ClientInfo.sin_family = AF_INET;
	m_SeverSocket.m_ClientInfo.sin_port = htons(static_cast<uint16_t>(PORT));
	inet_pton(AF_INET, Address.c_str(), &m_SeverSocket.m_ClientInfo.sin_addr);

	//바인딩
	if (::bind(m_SeverSocket.m_Socket, reinterpret_cast<sockaddr*>(&m_SeverSocket.m_ClientInfo), sizeof(m_SeverSocket.m_ClientInfo)) == SOCKET_ERROR)
		assert(false);

	if (::listen(m_SeverSocket.m_Socket, 10) == SOCKET_ERROR)
		assert(false);

	DataManager::Get()->Init();
	cout << "클라이언트 접속 대기중..." << endl;

	return true;
}

void IOCP::Run()
{
	SocketInfo* newInfo = NULLPTR;

	while (true)
	{
		SOCKET ClientSock;
		SOCKADDR_IN ClientAddr; 
		int AddrLen = sizeof(ClientAddr);

		ClientSock = ::accept(m_SeverSocket.m_Socket, reinterpret_cast<sockaddr*>(&ClientAddr), &AddrLen);

		if (ClientSock == INVALID_SOCKET)
			assert(false);

		cout << "클라이언트 접속!" << endl;

		newInfo = new SocketInfo();
		newInfo->m_Socket = ClientSock;
		newInfo->m_ClientInfo = ClientAddr;
		newInfo->m_CliendID = DataManager::m_ClientCount;
		DataManager::Get()->PushClient(newInfo);

		//Overlapped 소켓과 Completion Port 의 연결
		CreateIoCompletionPort((HANDLE)newInfo->m_Socket, m_CompletionPort, (ULONG_PTR)newInfo, 0);

		IO_Data* newData = new IO_Data();
		ZeroMemory(&newData->m_Overlapped, sizeof(newData->m_Overlapped));
		newData->m_WsaBuf.buf = newData->GetBuffer();
		newData->m_WsaBuf.len = BUFFERSIZE;
		newData->m_Mode = READ;

		//Overraped입출력 시작 의미
		DWORD Flags = 0;
		DWORD RecvBytes = 0;
		WSARecv(ClientSock, &newData->m_WsaBuf, 1, &RecvBytes, &Flags, &newData->m_Overlapped, NULLPTR);

		//새로 접속한 클라에 메인플레이어 생성
		Sever_SendNewPlayerMsg(newInfo);

		//기존 접속한 클라에 OT생성
		Sever_SendConnectClientNewOtherPlayer(newInfo);

		Sleep(500);
		//EatData전송
		Sever_SendSeeList(newInfo);
	}
}

void IOCP::ThreadFunc()
{
	DWORD ByteTransferred;
	IO_Data* IOData;
	char Buffer[BUFFERSIZE] = {};

	while (true)
	{
		//입출력이 완료된 소켓의 정보 얻음
		GetQueuedCompletionStatus(m_CompletionPort, (LPDWORD)&ByteTransferred, (PULONG_PTR)&m_SocketInfo, (LPOVERLAPPED*)&IOData, INFINITE);

		// 전송된 바이트가 0일때(소켓이 닫혔다는 의미)
		if (ByteTransferred == 0)
		{
			mutex Mutex;
			lock_guard<mutex> LockMutex(Mutex);

			Sever_DieClient(m_SocketInfo);
			SAFE_DELETE(IOData);
			continue;
		}

		lock_guard<mutex> myMutex(m_Mutex);

		int RWMode = IOData->m_Mode;
		IOData->CopyBuffer();
		memcpy(Buffer, IOData->GetBuffer(), IOData->GetSize());
		size_t Size = IOData->GetSize();

		SAFE_DELETE(IOData);

		if (RWMode == READ)
			SeverMesageProcess(m_SocketInfo, Buffer, Size);
	}
}

void IOCP::IOCPSeverSend(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;
	Data->m_Mode = WRITE;

	int getResult = WSASend(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, Flags, (LPOVERLAPPED)Data, NULLPTR);

	if (getResult != 0)
		cout << "Error : " << WSAGetLastError() << endl;
}

void IOCP::Sever_SendNewPlayerMsg(SocketInfo * Socket)
{
	int RandNum = Core::Get()->RandomRange(0, 139);
	Vector4 RandColor = Vector4::AllColor[RandNum];
	Vector3 Pos = Vector3(500.0f, 500.0f, 1.0f);
	float Scale = 10.0f;

	DataManager::Get()->PushPlayerInfo(RandColor, Pos, Socket->m_CliendID, Scale);

	IO_Data* IoData = new IO_Data();
	IoData->WriteHeader<CreateMainPlayerMessage>();
	IoData->WriteBuffer<size_t>(&Socket->m_CliendID);
	IoData->WriteBuffer<Vector4>(&RandColor);
	IoData->WriteBuffer<Vector3>(&Pos);
	IoData->WriteBuffer<float>(&Scale);

	cout << Socket->m_CliendID << "번 클라이언트에게 플레이어 생성메세지 전송" << endl << endl;

	size_t ClientCount = DataManager::Get()->GetClientCount();
	auto getPlayerVec = DataManager::Get()->GetPlayerVec();

	IoData->WriteBuffer<int>(&ClientCount);

	if (ClientCount == 1)
	{
		IOCPSeverSend(Socket, IoData);
		return;
	}

	//데이터를 보낸다
	for (auto Cur : *getPlayerVec)
	{
		if (Socket->m_CliendID == Cur->m_ClientID)
			continue;

		IoData->WriteBuffer(&Cur->m_Color, 16);
		IoData->WriteBuffer<Vector3>(&Cur->m_Pos);
		IoData->WriteBuffer<float>(&Cur->m_Scale);
		IoData->WriteBuffer<size_t>(&Cur->m_ClientID);
	}

	cout << Socket->m_CliendID << "번 클라이언트에 기존 접속한 클라이언트 갯수 : " << ClientCount - 1 << " 개 만큼 OtherPlayer 생성메세지 전송" << endl;

	IOCPSeverSend(Socket, IoData);
}

void IOCP::Sever_SendConnectClientNewOtherPlayer(SocketInfo * NewSocket)
{
	if (DataManager::Get()->GetClientCount() == 0 || DataManager::Get()->GetClientCount() == 1)
		return;

	auto getVec = DataManager::Get()->GetPlayerVec();

	if (getVec->size() == 0 || getVec->size() == 1)
		return;

	PlayerInfo* getInfo = getVec->at(getVec->size() - 1);

	cout << NewSocket->m_CliendID << "번 클라이언트에게 OtherPlayer 생성메세지 전송" << endl;

	IO_Data* IoData = new IO_Data();
	IoData->WriteHeader<CreateConnectClientCreateOtherPlayer>();
	IoData->WriteBuffer<size_t>(&getInfo->m_ClientID);
	IoData->WriteBuffer<Vector4>(&getInfo->m_Color);
	IoData->WriteBuffer<Vector3>(&getInfo->m_Pos);
	IoData->WriteBuffer<Vector3>(&getInfo->m_Scale);

	for (auto Cur : *DataManager::Get()->GetClientVec())
	{
		if (Cur->m_Socket == NewSocket->m_Socket)
			continue;

		IOCPSeverSend(Cur, IoData);
	}
}

void IOCP::SeverMesageProcess(SocketInfo * Socket, char * Data, size_t BufferSize)
{
	ReadMemoryStream Reader(Data, BufferSize);

	if (BufferSize == 0)
		return;

	m_State = static_cast<SEVER_DATA_TYPE>(Reader.Read<int>());

	switch (m_State)
	{
	case SST_PLAYER_POS:
		Sever_UpdatePos(Socket, Reader);
		break;
	case SST_PLAYER_SCALE:
		Sever_UpdateScale(Socket, Reader);
		break;
	}

	RecvInitIOData(Socket);
}

void IOCP::Sever_DieClient(SocketInfo * Socket)
{
	mutex Mutex;
	size_t DeleteID = Socket->m_CliendID;

	Mutex.lock();
	cout << DeleteID << "번 클라이언트 종료" << endl;

	Sever_SendDeleteOT(Socket);
	DataManager::Get()->DeleteSocket(Socket);

	Mutex.unlock();
	m_State = SST_NONE;
}

void IOCP::Sever_SendDeleteOT(SocketInfo * Socket)
{
	auto getVec = DataManager::Get()->GetClientVec();

	size_t DeleteID = Socket->m_CliendID;

	IO_Data* IoData = new IO_Data();
	IoData->WriteHeader<OtherPlayerDelete>();
	IoData->WriteBuffer<size_t>(&DeleteID);

	mutex Mutex;
	lock_guard<mutex> LockMutex(Mutex);

	for (auto CurClient : *getVec)
	{
		if (CurClient->m_Socket == Socket->m_Socket)
			continue;

		IOCPSeverSend(CurClient, IoData);
	}
}

void IOCP::Sever_UpdatePos(SocketInfo * Socket, ReadMemoryStream & Reader)
{
	size_t ReadID = Reader.Read<size_t>();
	Vector3 Pos = Reader.Read<Vector3>();

	auto getInfo = DataManager::Get()->FindPlayerInfoKey(Socket->m_CliendID);

	if (getInfo == NULLPTR)
	{
		cout << "Error! 서버에 플레이어 데이터가 없습니다." << endl;
		TrueAssert(true);
		return;
	}

	getInfo->m_Pos = Pos;

	Sever_SendPlayerPos(Socket);
}

void IOCP::Sever_UpdateScale(SocketInfo * Socket, ReadMemoryStream & Reader)
{
	size_t ReadID = Reader.Read<size_t>();
	float Scale = Reader.Read<float>();

	auto getInfo = DataManager::Get()->FindPlayerInfoKey(ReadID);
	getInfo->m_Scale = Scale;

	Sever_SendPlayerScale(Socket, Scale);
}

void IOCP::Sever_SendPlayerPos(SocketInfo * Socket)
{
	auto getVec = DataManager::Get()->GetClientVec();

	if (getVec->size() == 0 || getVec->size() == 1)
		return;

	static int TempFrame = 0;
	TempFrame++;

	if (TempFrame >= 7)
	{
		TempFrame = 0;

		IO_Data* IoData = new IO_Data();
		IoData->WriteHeader<PlayerPosMessage>();
		IoData->WriteBuffer<size_t>(&Socket->m_CliendID);

		auto getInfo = DataManager::Get()->FindPlayerInfoKey(Socket->m_CliendID);
		IoData->WriteBuffer<Vector3>(&getInfo->m_Pos);

		for (auto CurClient : *getVec)
		{
			if (CurClient->m_Socket == Socket->m_Socket)
				continue;

			IOCPSeverSend(CurClient, IoData);
		}
	}
}

void IOCP::Sever_SendPlayerScale(SocketInfo * Socket, float Scale)
{
	auto getVec = DataManager::Get()->GetClientVec();

	if (getVec->size() == 0 || getVec->size() == 1)
		return;

	IO_Data* IoData = new IO_Data();
	IoData->WriteHeader<PlayerScaleMessage>();
	IoData->WriteBuffer<size_t>(&Socket->m_CliendID);
	IoData->WriteBuffer<float>(&Scale);

	for (auto CurClient : *getVec)
	{
		if (CurClient->m_Socket == Socket->m_Socket)
			continue;

		IOCPSeverSend(CurClient, IoData);
	}
}

void IOCP::RecvInitIOData(SocketInfo * Info)
{
	IO_Data* IoData = new IO_Data();
	ZeroMemory(&IoData->m_Overlapped, sizeof(OVERLAPPED));
	IoData->m_WsaBuf.buf = IoData->GetBuffer();
	IoData->m_WsaBuf.len = BUFFERSIZE;
	IoData->m_Mode = READ;

	DWORD RecvByte = 0;
	DWORD Flag = 0;

	WSARecv(Info->m_Socket, &IoData->m_WsaBuf, 1, &RecvByte, &Flag, &IoData->m_Overlapped, NULLPTR);
}

SEVER_DATA_TYPE IOCP::ReadHeader(char * Buffer)
{
	SEVER_DATA_TYPE HeaderType = SST_NONE;
	memcpy(&HeaderType, Buffer, sizeof(Header));

	return HeaderType;
}

SEVER_DATA_TYPE IOCP::IOCPSeverRecvMsg(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;
	SEVER_DATA_TYPE HeaderType = SST_NONE;

	ZeroMemory(&Data->m_Overlapped, sizeof(Data->m_Overlapped));
	int RecvByte = WSARecv(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, &Flags, &Data->m_Overlapped, NULLPTR);

	if (WSAGetLastError() != WSA_IO_PENDING)
	{
		m_State = HeaderType;
		return m_State;
	}

	Data->CopyBuffer();

	if (Data->m_WsaBuf.buf == NULLPTR)
	{
		m_State = HeaderType;
		return m_State;
	}

	HeaderType = Data->ReadHeader();

	if (Data->m_WsaBuf.len == 0)
	{
		m_State = HeaderType;
		return m_State;
	}

	m_State = HeaderType;
	Data->HeaderErase();

	return HeaderType;
}

void IOCP::Sever_SendSeeList(SocketInfo * Socket)
{
	auto getEatVec = DataManager::Get()->GetEatVec();
	vector<EatInfo*> SendList;
	SendList.reserve(50);

	for (auto CurEat : *getEatVec)
	{
		//시야판단
		//초기위치
		Vector3 Origin = Vector3(0.0f, 500.0f, 0);
		Vector3 EatPos = CurEat->Pos;

		if ((Origin.x <= EatPos.x && Origin.y <= EatPos.y ) && (Origin.x + 1280.0f >= EatPos.x && Origin.y + 720.0f >= EatPos.y))
			SendList.push_back(CurEat);
	}

	size_t ListSize = SendList.size();

	IO_Data* newData = new IO_Data();
	newData->WriteHeader<CreateEatObjectMessage>();
	newData->WriteBuffer<size_t>(&ListSize);

	for (auto CurEat : SendList)
	{
		newData->WriteBuffer<Vector3>(&CurEat->Pos);
		newData->WriteBuffer<Vector4>(&CurEat->Color);
		newData->WriteBuffer<int>(&CurEat->ID);
	}

	IOCPSeverSend(Socket, newData);
}