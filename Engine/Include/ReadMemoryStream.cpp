#include "stdafx.h"
#include "ReadMemoryStream.h"

JEONG_USING

ReadMemoryStream::ReadMemoryStream(char* InputBuffer, size_t BufferSize)
{
	m_ReadBuffer = InputBuffer;
	m_BufferSize = BufferSize;
}

ReadMemoryStream::~ReadMemoryStream()
{
	delete[] m_ReadBuffer;
	m_ReadBuffer = nullptr;
}
