#include "stdafx.h"
#include "ReadMemoryStream.h"

ReadMemoryStream::ReadMemoryStream(char* InputBuffer)
{
	m_ReadBuffer = InputBuffer;
	m_Size = 0;
	m_Capacity = 0;
	m_isInit = false;
}

ReadMemoryStream::~ReadMemoryStream()
{
	delete[] m_ReadBuffer;
	m_ReadBuffer = nullptr;
}

void ReadMemoryStream::Read(void* Buffer, size_t length)
{
	m_Size += length;
	memcpy(Buffer, m_ReadBuffer + m_Size, length);
}

