
#ifndef PCH_H
#define PCH_H

#include <WinSock2.h>
#include <mswsock.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <crtdbg.h>
#include <conio.h>
#include <assert.h>
#include <array>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <atlstr.h>
#include <process.h>

#include <MathHeader.h>

using namespace std;
#define PORT 15000
#define BUFFERSIZE 2048

static size_t ClientCount = 0;

enum class WRITE_READ_MODE
{
	WR_READ,
	WR_WRITE,
	WR_NONE,
};

struct SocketInfo
{
	SOCKET m_Socket;
	SOCKADDR_IN m_ClientInfo;
	size_t m_CliendID = 0;
};

struct IO_Data
{
	OVERLAPPED m_Overlapped;
	char m_Buffer[BUFFERSIZE] = {};
	WSABUF m_WsaBuf;
	WRITE_READ_MODE m_RWMode = WRITE_READ_MODE::WR_NONE;
};

#endif //PCH_H
