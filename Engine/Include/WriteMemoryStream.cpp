#include "stdafx.h"
#include "WriteMemoryStream.h"

JEONG_USING

WriteMemoryStream::WriteMemoryStream()
{
	m_Size = 0;
	m_Capacity = BUFFERSIZE;

	m_Buffer = new char[BUFFERSIZE];
	ZeroMemory(m_Buffer, BUFFERSIZE);
}

WriteMemoryStream::WriteMemoryStream(size_t Capacity)
{
	m_Capacity = Capacity + 48;
	m_Size = 0;

	m_Buffer = new char[Capacity + 48];
	ZeroMemory(m_Buffer, Capacity + 48);
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

void WriteMemoryStream::InsertByteSize()
{
	m_Size += 4;

	char* TempBuf = new char[m_Size - 4];
	memcpy(TempBuf, m_Buffer + 4, m_Size - 4);


	SAFE_DELETE_ARRARY(TempBuf);
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
