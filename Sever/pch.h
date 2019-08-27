
#ifndef PCH_H
#define PCH_H

#include <WinSock2.h>
#include <mswsock.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#pragma comment(lib, "Mathlib64_Debug.lib")
#pragma comment(lib, "Mathlib64.lib")

#pragma comment(lib, "Engine64_Debug.lib")
#pragma comment(lib, "Engine64.lib")

#include <stdafx.h>
#include <MessageManager.h>

#include <MathHeader.h>
#include <Vector4.h>
#include <Vector3.h>

using namespace std;

#endif //PCH_H
