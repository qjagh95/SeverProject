#include "pch.h"
#include "IOCP.h"
#include "PlayerInfo.h"
#include "DataManager.h"

IOCP::IOCP()
{
	closesocket(m_SeverSocket.m_Socket);
	WSACleanup();
	m_SeverSocket.m_CliendID = DataManager::m_ClientCount;
	DataManager::m_ClientCount++;
}

IOCP::~IOCP()
{
	for (size_t i = 0; i < m_vecThread.size(); i++)
		m_vecThread[i]->join();

	for (size_t i = 0; i < m_vecThread.size(); i++)
	{
		if (m_vecThread[i] != nullptr)
		{
			delete m_vecThread[i];
			m_vecThread[i] = nullptr;
		}
	}
}

bool IOCP::Init()
{
	WSAData Buf;
	SYSTEM_INFO Info;

	if (WSAStartup(MAKEWORD(2, 2), &Buf) != 0)
		assert(false);

	//CompleationPort 생성
	m_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	GetSystemInfo(&Info);

	//CPU갯수 * 2만큼 스레드 생성
	for (size_t i = 0; i < Info.dwNumberOfProcessors; i++)
		m_vecThread.push_back(new thread(&IOCP::ThreadFunc, this));

	SetSocket();

	return true;
}

void IOCP::SetSocket()
{
	string Address = "192.168.1.172";

	m_SeverSocket.m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (m_SeverSocket.m_Socket == INVALID_SOCKET)
		assert(false);

	// 바인딩할 소켓 주소정보
	m_SeverSocket.m_ClientInfo.sin_family = AF_INET;
	m_SeverSocket.m_ClientInfo.sin_port = htons((uint16_t)PORT);
	inet_pton(AF_INET, Address.c_str(), &m_SeverSocket.m_ClientInfo.sin_addr);

	//바인딩
	if (::bind(m_SeverSocket.m_Socket, reinterpret_cast<sockaddr*>(&m_SeverSocket.m_ClientInfo), sizeof(m_SeverSocket.m_ClientInfo)) == SOCKET_ERROR)
		assert(false);

	if (::listen(m_SeverSocket.m_Socket, 10) == SOCKET_ERROR)
		assert(false);
}

void IOCP::Run()
{
	SocketInfo* newInfo; 
	IO_Data* IoData;

	DWORD RecvByte;
	DWORD Flags;

	while (true)
	{
		SOCKET ClientSock;
		SOCKADDR_IN ClientAddr; 
		int AddrLen = sizeof(ClientAddr);

		ClientSock = ::accept(m_SeverSocket.m_Socket, reinterpret_cast<sockaddr*>(&ClientAddr), &AddrLen);

		if (ClientSock == INVALID_SOCKET)
			assert(false);

		newInfo = new SocketInfo();
		newInfo->m_Socket = ClientSock;
		newInfo->m_ClientInfo = ClientAddr;
		newInfo->m_CliendID = DataManager::m_ClientCount;
		DataManager::Get()->PushClient(newInfo);

		//Overlapped 소켓과 Completion Port 의 연결
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(newInfo->m_Socket), m_CompletionPort, reinterpret_cast<DWORD>(newInfo), 0);

		// 클라이언트를 위한 버퍼를 설정, OVERLAPPED 변수 초기화
		IoData = new IO_Data();
		memset(&(IoData->m_Overlapped), 0, sizeof(OVERLAPPED));
		IoData->m_WsaBuf.len = BUFFERSIZE;
		IoData->m_WsaBuf.buf = IoData->m_Buffer;
		Flags = 0;

		DataManager::m_ClientCount++;

		//Client접속처리 후 플레이어 생성메세지를 던진다.
		//중첩된 데이터입력
		//WSARecv(newInfo->m_Socket, &(IoData->m_WsaBuf), 1, reinterpret_cast<LPDWORD>(&RecvByte), 
		//reinterpret_cast<LPDWORD>(&Flags), &(IoData->m_Overlapped), NULL);
	}
}

void IOCP::ThreadFunc()
{
	HANDLE CompletionPort = reinterpret_cast<HANDLE>(m_CompletionPort); \
	DWORD ByteTransferred;
	SocketInfo* SocketData;
	IO_Data* IoData;

	while (true)
	{
		//입.출력이 완료된 소켓의 정보 얻음
		GetQueuedCompletionStatus(CompletionPort, reinterpret_cast<LPDWORD>(&ByteTransferred), 
		reinterpret_cast<PULONG_PTR>(&SocketData), reinterpret_cast<OVERLAPPED**>(&IoData), 0);

		// 전송된 바이트가 0일때 종료 (EOF 전송 시에도) 
		if (ByteTransferred == 0) 
		{ 
			closesocket(SocketData->m_Socket);
			DataManager::Get()->DeleteSocket(SocketData);
			delete IoData;

			continue;
		}

		DataManager::Get()->MessageProcess(SocketData, IoData); 
	}
}
