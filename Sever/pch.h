﻿
#ifndef PCH_H
#define PCH_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <mswsock.h>
#include <Ws2tcpip.h>
#include <memory.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#pragma comment(lib, "Mathlib64_Debug.lib")
#pragma comment(lib, "Mathlib64.lib")

#pragma comment(lib, "Engine64_Debug.lib")
#pragma comment(lib, "Engine64.lib")

#include <stdafx.h>

#include <MathHeader.h>
#include <Vector4.h>
#include <Vector3.h>

#include <sqlext.h>

using namespace std;

struct EatInfo
{
	int ID;
	Vector3 Pos;
	Vector4 Color;

	EatInfo()
	{
		ID = -1;
		Pos = Vector3::Zero;
		Color = Vector4::Zero;
	}
};

#endif //PCH_H
