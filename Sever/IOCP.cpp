#include "pch.h"
#include "IOCP.h"

#include <DataManager.h>
#include <WriteMemoryStream.h>
#include <ReadMemoryStream.h>
#include <Core.h>

JEONG_USING

IOCP::IOCP()
{
	m_SeverSocket.m_CliendID = DataManager::m_ClientCount;
	m_IOData = NULLPTR;
}

IOCP::~IOCP()
{
	Core::Delete();

	for (size_t i = 0; i < m_vecThread.size(); i++)
		m_vecThread[i]->join();

	Safe_Delete_VecList(m_vecThread);
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

	SetSocket();

	cout << "클라 접속 대기중..." << endl;

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
		newData->m_WsaBuf.len = newData->GetSize();

		//Overraped입출력 시작 의미
		DWORD Flags = 0;
		LPDWORD RecvBytes = 0;
		WSARecv(newInfo->m_Socket, &newData->m_WsaBuf, 1, RecvBytes, &Flags, &newData->m_Overlapped, NULLPTR);

		//새로 접속한 클라에 메인플레이어 생성
		MessageManager::Get()->Sever_SendNewPlayerMsg(newInfo);

		//기존 접속한 클라에 OT생성
		MessageManager::Get()->Sever_SendConnectClientNewOtherPlayer(newInfo);
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
		if (GetQueuedCompletionStatus(m_CompletionPort, (LPDWORD)&ByteTransferred, (PULONG_PTR)&m_SocketInfo,
			(LPOVERLAPPED*)&IOData, INFINITE) == FALSE)
			continue;

		// 전송된 바이트가 0일때(소켓이 닫혔다는 의미)
		if (ByteTransferred == 0)
		{
			MessageManager::Get()->Sever_DieClient(m_SocketInfo);
			SAFE_DELETE(IOData);
			continue;
		}

		memcpy(Buffer, IOData->GetBuffer(), IOData->GetSize());
		size_t Size = IOData->GetSize();
		SAFE_DELETE(IOData);

		lock_guard<mutex> Mutex(m_Mutex);
		MessageManager::Get()->SeverMesageProcess(m_SocketInfo, Buffer, Size);
	}
}

void IOCP::IOCPSeverSend(SocketInfo * Socket, IO_Data * Data)
{
}