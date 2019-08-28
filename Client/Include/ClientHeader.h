#pragma once
#include <stdafx.h>

#define PORT 15000

JEONG_USING

enum USERCOMPONENT_TYPE
{
	UT_NONE = CT_MAX,
	UT_PLAYER,
	UT_EAT_OBJECT,
	UT_OTHER_PLAYER,
};