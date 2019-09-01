#include "pch.h"
#include "IOCP.h"

#include <DataManager.h>
#include <WriteMemoryStream.h>

JEONG_USING

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

	//CompleationPort ����
	m_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULLPTR, 0, 0);
	GetSystemInfo(&Info);

	//CPU���� * 2��ŭ ������ ����
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
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(newInfo->m_Socket), m_CompletionPort, reinterpret_cast<DWORD>(newInfo), 0);

		//Ŭ������޼����� ������.
		MessageManager::Get()->Sever_SendNewPlayerMsg(newInfo);

		DataManager::Get()->PushClient(newInfo);
		DataManager::m_ClientCount++;
	}
}

void IOCP::ThreadFunc()
{
	HANDLE CompletionPort = reinterpret_cast<HANDLE>(m_CompletionPort);
	DWORD ByteTransferred;
	SocketInfo* SocketData = NULLPTR;
	IO_Data* IoData = NULLPTR;

	while (true)
	{
		//��.����� �Ϸ�� ������ ���� ����
		GetQueuedCompletionStatus(CompletionPort, reinterpret_cast<LPDWORD>(&ByteTransferred), 
		reinterpret_cast<PULONG_PTR>(&SocketData), reinterpret_cast<OVERLAPPED**>(&IoData), 0);

		// ���۵� ����Ʈ�� 0�϶� ���� (EOF ���� �ÿ���) 
		if (ByteTransferred == 0) 
		{ 
			closesocket(SocketData->m_Socket);
			cout << SocketData->m_CliendID << "�� Ŭ���̾�Ʈ ����" << endl;
			DataManager::Get()->DeleteSocket(SocketData);
			DataManager::m_ClientCount--;
			delete IoData;

			continue;
		}

		if (SocketData != NULLPTR && IoData != NULLPTR)
			MessageManager::Get()->SeverMesageProcess(SocketData, IoData);
	}
}
