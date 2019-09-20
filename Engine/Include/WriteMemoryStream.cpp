#include "stdafx.h"
#include "WriteMemoryStream.h"

JEONG_USING

WriteMemoryStream::WriteMemoryStream()
{
	m_Size = 0;
	m_Capacity = BUFFERSIZE;

	m_Buffer = new char[BUFFERSIZE];
	memset(m_Buffer, 0, BUFFERSIZE);
}

WriteMemoryStream::~WriteMemoryStream()
{
	SAFE_DELETE_ARRARY(m_Buffer);
}

void WriteMemoryStream::Write(const void * Data, size_t Length)
{
	if (m_Size + Length > m_Capacity)
		Resize();

	memcpy(m_Buffer + m_Size, Data, Length);
	m_Size += Length;
}

char * WriteMemoryStream::GetBuffer()
{
	return m_Buffer;
}

void WriteMemoryStream::HeaderErase()
{
	if (m_Size == 0)
		return;

	memcpy(m_Buffer, m_Buffer + 4, m_Size - 4);
}

void WriteMemoryStream::BufferClear()
{
	m_Size = 0;
}

void WriteMemoryStream::PullBuffer(size_t Size)
{
	if (m_Size == 0 || m_Size - Size < 0)
		return;

	memcpy(m_Buffer, m_Buffer + Size, m_Size - Size);
}

void WriteMemoryStream::Resize()
{
	m_Capacity *= 2;

	char* newBuffer = new char[m_Capacity];
	memcpy(newBuffer, m_Buffer, m_Size);
	SAFE_DELETE(m_Buffer);

	m_Buffer = newBuffer;
}
