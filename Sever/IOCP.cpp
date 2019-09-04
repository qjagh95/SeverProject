#include "pch.h"
#include "IOCP.h"

#include <DataManager.h>
#include <WriteMemoryStream.h>

JEONG_USING

IOCP::IOCP()
{
	m_SeverSocket.m_CliendID = DataManager::m_ClientCount;
	DataManager::m_ClientCount++;

	m_SocketInfo = new SocketInfo();
	m_SocketInfo->m_Socket = 0;
	m_SocketInfo->m_CliendID = -1;
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

	SAFE_DELETE(m_SocketInfo);
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
	for (size_t i = 0; i < Info.dwNumberOfProcessors; i++)
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

		//새로 접속한 클라에 메인플레이어 생성
		MessageManager::Get()->Sever_SendNewPlayerMsg(newInfo);

		//기존 접속한 클라에 OT생성
		MessageManager::Get()->Sever_SendConnectClientNewOtherPlayer(newInfo);

		//새롭게 접속한 클라에 현재 접속한 클라갯수만큼 OT생성 명령
	}
}

void IOCP::ThreadFunc()
{
	HANDLE CompletionPort = reinterpret_cast<HANDLE>(m_CompletionPort);
	DWORD ByteTransferred;

	while (true)
	{
		//입출력이 완료된 소켓의 정보 얻음
		if (GetQueuedCompletionStatus(CompletionPort, (LPDWORD)&ByteTransferred, (PULONG_PTR)&m_SocketInfo, 
			(LPOVERLAPPED*)&m_IOData, INFINITE) == FALSE)
			continue;

		// 전송된 바이트가 0일때 종료 (EOF 전송 시에도) 
		if (ByteTransferred == 0)
		{
			cout << m_SocketInfo->m_CliendID << "번 클라이언트 종료" << endl;

			MessageManager::Get()->Sever_DieClient(m_SocketInfo);
			m_IOData->ClearBuffer();
			continue;
		}

		if (m_SocketInfo->m_Socket != 0 && m_IOData->m_WsaBuf.len != 0)
			MessageManager::Get()->SeverMesageProcess(m_SocketInfo, m_IOData);
	}
}
