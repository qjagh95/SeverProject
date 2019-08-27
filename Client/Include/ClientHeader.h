#pragma once
#include <WinSock2.h>
#include <mswsock.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#include "stdafx.h"
#define PORT 15000

JEONG_USING

enum USERCOMPONENT_TYPE
{
	UT_NONE = CT_MAX,
	UT_PLAYER,
	UT_EAT_OBJECT,
	UT_OTHER_PLAYER,
};