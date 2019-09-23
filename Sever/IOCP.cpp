#include "pch.h"
#include "IOCP.h"

#include <DataManager.h>
#include <WriteMemoryStream.h>
#include <ReadMemoryStream.h>
#include <Core.h>

JEONG_USING

IOCP::IOCP()
{
	m_State = SST_NONE;
	m_SeverSocket.m_CliendID = DataManager::m_ClientCount;
}

IOCP::~IOCP()
{
	Core::Delete();

	for (size_t i = 0; i < m_vecThread.size(); i++)
		m_vecThread[i]->join();

	Safe_Delete_VecList(m_vecThread);

	DataManager::Get()->CloseAll();
	closesocket(m_SeverSocket.m_Socket);
	WSACleanup();
}

bool IOCP::Init()
{
	WSAData Buf;
	SYSTEM_INFO Info;

	if (WSAStartup(MAKEWORD(2, 2), &Buf) != 0)
		assert(false);

	//CompleationPort ����
	m_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULLPTR, 0, 0);
	GetSystemInfo(&Info);

	//CPU���� ��ŭ ������ ����
	for (size_t i = 0; i < Info.dwNumberOfProcessors * 2; i++)
		m_vecThread.push_back(new thread(&IOCP::ThreadFunc, this));

	SetSocket();

	cout << "Ŭ�� ���� �����..." << endl;

	return true;
}

void IOCP::SetSocket()
{
	string Address = "192.168.1.172";

	m_SeverSocket.m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (m_SeverSocket.m_Socket == INVALID_SOCKET)
		assert(false);

	// ���ε��� ���� �ּ�����
	m_SeverSocket.m_ClientInfo.sin_family = AF_INET;
	m_SeverSocket.m_ClientInfo.sin_port = htons((uint16_t)PORT);
	inet_pton(AF_INET, Address.c_str(), &m_SeverSocket.m_ClientInfo.sin_addr);

	//���ε�
	if (::bind(m_SeverSocket.m_Socket, reinterpret_cast<sockaddr*>(&m_SeverSocket.m_ClientInfo), sizeof(m_SeverSocket.m_ClientInfo)) == SOCKET_ERROR)
		assert(false);

	if (::listen(m_SeverSocket.m_Socket, 10) == SOCKET_ERROR)
		assert(false);
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

		cout << "Ŭ���̾�Ʈ ����!" << endl;

		newInfo = new SocketInfo();
		newInfo->m_Socket = ClientSock;
		newInfo->m_ClientInfo = ClientAddr;
		newInfo->m_CliendID = DataManager::m_ClientCount;
		DataManager::Get()->PushClient(newInfo);

		//Overlapped ���ϰ� Completion Port �� ����
		CreateIoCompletionPort((HANDLE)newInfo->m_Socket, m_CompletionPort, (ULONG_PTR)newInfo, 0);

		IO_Data* newData = new IO_Data();
		ZeroMemory(&newData->m_Overlapped, sizeof(newData->m_Overlapped));
		newData->m_WsaBuf.buf = newData->GetBuffer();
		newData->m_WsaBuf.len = newData->GetSize();

		//Overraped����� ���� �ǹ�
		DWORD Flags = 0;
		LPDWORD RecvBytes = 0;
		WSARecv(ClientSock, &newData->m_WsaBuf, 1, RecvBytes, &Flags, &newData->m_Overlapped, NULLPTR);

		//���� ������ Ŭ�� �����÷��̾� ����
		Sever_SendNewPlayerMsg(newInfo);

		//���� ������ Ŭ�� OT����
		Sever_SendConnectClientNewOtherPlayer(newInfo);
	}
}

void IOCP::ThreadFunc()
{
	DWORD ByteTransferred;
	IO_Data* IOData;
	char Buffer[BUFFERSIZE] = {};

	while (true)
	{
		//������� �Ϸ�� ������ ���� ����
		GetQueuedCompletionStatus(m_CompletionPort, (LPDWORD)&ByteTransferred, (PULONG_PTR)&m_SocketInfo, (LPOVERLAPPED*)&IOData, INFINITE);

		// ���۵� ����Ʈ�� 0�϶�(������ �����ٴ� �ǹ�)
		if (ByteTransferred == 0)
		{
			mutex Mutex;
			Mutex.lock();

			Sever_DieClient(m_SocketInfo);
			SAFE_DELETE(IOData);

			Mutex.unlock();
			continue;
		}

		memcpy(Buffer, IOData->GetBuffer(), IOData->GetSize());
		size_t Size = IOData->GetSize();
		SAFE_DELETE(IOData);

		lock_guard<mutex> Mutex(m_Mutex);
		SeverMesageProcess(m_SocketInfo, Buffer, Size);
	}
}

void IOCP::IOCPSeverSend(SocketInfo * Socket, IO_Data * Data)
{
	DWORD Flags = 0;

	int getResult = WSASend(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, Flags, (LPOVERLAPPED)Data, NULLPTR);

	if (getResult != 0)
	{
		int a = WSAGetLastError();
		cout << "Error : " << a << endl;
	}
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

	cout << Socket->m_CliendID << "�� Ŭ���̾�Ʈ���� �÷��̾� �����޼��� ����" << endl << endl;

	int ClientCount = DataManager::Get()->GetClientCount();
	auto getPlayerVec = DataManager::Get()->GetPlayerVec();

	IoData->WriteBuffer<int>(&ClientCount);

	if (ClientCount == 1)
	{
		IOCPSeverSend(Socket, IoData);
		return;
	}

	//�����͸� ������
	for (auto Cur : *getPlayerVec)
	{
		if (Socket->m_CliendID == Cur->m_ClientID)
			continue;

		IoData->WriteBuffer(&Cur->m_Color, 16);
		IoData->WriteBuffer<Vector3>(&Cur->m_Pos);
		IoData->WriteBuffer<float>(&Cur->m_Scale);
		IoData->WriteBuffer<size_t>(&Cur->m_ClientID);
	}

	cout << Socket->m_CliendID << "�� Ŭ���̾�Ʈ�� ���� ������ Ŭ���̾�Ʈ ���� : " << ClientCount - 1 << " �� ��ŭ OtherPlayer �����޼��� ����" << endl;

	IOCPSeverSend(Socket, IoData);
}

bool IOCP::Sever_SendConnectClientNewOtherPlayer(SocketInfo * NewSocket)
{
	if (DataManager::Get()->GetClientCount() == 0 || DataManager::Get()->GetClientCount() == 1)
		return false;

	auto getVec = DataManager::Get()->GetPlayerVec();

	if (getVec->size() == 0 || getVec->size() == 1)
		return false;

	PlayerInfo* getInfo = getVec->at(getVec->size() - 1);

	cout << NewSocket->m_CliendID << "�� Ŭ���̾�Ʈ���� OtherPlayer �����޼��� ����" << endl;

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

bool IOCP::SeverMesageProcess(SocketInfo * Socket, char * Data, size_t BufferSize)
{
	ReadMemoryStream Reader(Data, BufferSize);

	if (BufferSize == 0)
		return false;

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

	InitIOData(Socket);

	//m_State = IOCPSeverRecvMsg(Socket, Data);

	//if (m_State != SST_NONE)
	//{
	//	InitIOData(Socket, Data);
	//}
}

void IOCP::Sever_DieClient(SocketInfo * Socket)
{
	mutex Mutex;
	size_t DeleteID = Socket->m_CliendID;

	cout << DeleteID << "�� Ŭ���̾�Ʈ ����" << endl;

	Mutex.lock();

	Sever_SendDeleteOT(Socket);
	DataManager::Get()->DeleteSocket(Socket);

	Mutex.unlock();
	m_State = SST_NONE;
}

void IOCP::Sever_SendDeleteOT(SocketInfo * Socket)
{
	auto getVec = DataManager::Get()->GetClientVec();

	size_t DeleteID = Socket->m_CliendID;

	IO_Data IoData = {};
	IoData.WriteHeader<OtherPlayerDelete>();
	IoData.WriteBuffer<size_t>(&DeleteID);

	mutex Mutex;
	lock_guard<mutex> LockMutex(Mutex);

	for (auto CurClient : *getVec)
	{
		if (CurClient->m_Socket == Socket->m_Socket)
			continue;

		IOCPSeverSend(CurClient, &IoData);
	}
}

void IOCP::Sever_UpdatePos(SocketInfo * Socket, ReadMemoryStream & Reader)
{
	size_t ReadID = Reader.Read<size_t>();
	Vector3 Pos = Reader.Read<Vector3>();

	auto getInfo = DataManager::Get()->FindPlayerInfoKey(Socket->m_CliendID);
	getInfo->m_Pos = Pos;

	Sever_SendPlayerPos(Socket, Pos);
}

void IOCP::Sever_UpdateScale(SocketInfo * Socket, ReadMemoryStream & Reader)
{
	size_t ReadID = Reader.Read<size_t>();
	float Scale = Reader.Read<float>();

	auto getInfo = DataManager::Get()->FindPlayerInfoKey(ReadID);
	getInfo->m_Scale = Scale;

	Sever_SendPlayerScale(Socket, Scale);
}

void IOCP::Sever_SendPlayerPos(SocketInfo * Socket, const Vector3 & Pos)
{
	auto getVec = DataManager::Get()->GetClientVec();

	if (getVec->size() == 0 || getVec->size() == 1)
		return;

	IO_Data IoData = {};
	IoData.WriteHeader<PlayerPos>();
	IoData.WriteBuffer<size_t>(&Socket->m_CliendID);
	IoData.WriteBuffer<Vector3>(&Pos);

	for (auto CurClient : *getVec)
	{
		if (CurClient->m_Socket == Socket->m_Socket)
			continue;

		IOCPSeverSend(CurClient, &IoData);
	}
}

void IOCP::Sever_SendPlayerScale(SocketInfo * Socket, float Scale)
{
	auto getVec = DataManager::Get()->GetClientVec();

	if (getVec->size() == 0 || getVec->size() == 1)
		return;

	IO_Data IoData = {};
	IoData.WriteHeader<PlayerPos>();
	IoData.WriteBuffer<size_t>(&Socket->m_CliendID);
	IoData.WriteBuffer<float>(&Scale);

	for (auto CurClient : *getVec)
	{
		if (CurClient->m_Socket == Socket->m_Socket)
			continue;

		IOCPSeverSend(CurClient, &IoData);
	}
}

void IOCP::InitIOData(SocketInfo * Info)
{
	IO_Data* newIOData = new IO_Data();
	ZeroMemory(&newIOData->m_Overlapped, sizeof(OVERLAPPED));
	newIOData->m_WsaBuf.buf = newIOData->GetBuffer();
	newIOData->m_WsaBuf.len = newIOData->GetSize();

	DWORD RecvByte = 0;
	DWORD Flag = 0;

	WSARecv(Info->m_Socket, &newIOData->m_WsaBuf, 1, &RecvByte, &Flag, &newIOData->m_Overlapped, NULLPTR);
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
	int a = WSARecv(Socket->m_Socket, &Data->m_WsaBuf, 1, NULLPTR, &Flags, &Data->m_Overlapped, NULLPTR);
	int b = WSAGetLastError();

	if (b != WSA_IO_PENDING)
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
