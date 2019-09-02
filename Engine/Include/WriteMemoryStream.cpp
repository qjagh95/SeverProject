#include "stdafx.h"
#include "WriteMemoryStream.h"

JEONG_USING

WriteMemoryStream::WriteMemoryStream()
{
	m_Capacity = 0;
	m_Size = 0;

	m_isInit = false;
}

WriteMemoryStream::~WriteMemoryStream()
{
	delete[] m_WriteBuffer;
	m_WriteBuffer = nullptr;
}

void WriteMemoryStream::Write(const void * Data, size_t Length)
{
	char* TempData = (char*)Data;

	if (m_isInit == false)
	{
		m_WriteBuffer = new char[Length + 1];
		m_Capacity = Length;
		Resize();
	}

	for (size_t i = 0; i < Length; i++)
	{
		m_WriteBuffer[m_Size] = TempData[i];
		m_Size++;

		Resize();
	}

	m_isInit = true;
}

void WriteMemoryStream::HeaderErase()
{
	memcpy(m_WriteBuffer, m_WriteBuffer + 4, m_Size - 4);
	m_Size -= 4;
}

void WriteMemoryStream::BufferClear()
{
	if (m_Size == 0)
		return;

	SAFE_DELETE_ARRARY(m_WriteBuffer);
}

void WriteMemoryStream::PullBuffer(size_t Size)
{
	if (m_Size - Size < 0)
		return;

	memcpy(m_WriteBuffer, m_WriteBuffer + Size, m_Size - Size);
}

char * WriteMemoryStream::ReadBuffer(size_t Size)
{
	if (Size == 0 || m_Size == 0)
		return NULLPTR;

	char* Buffer;

	memcpy(Buffer, m_WriteBuffer, Size);

	return Buffer;
}

//©жемаЭ?
void WriteMemoryStream::Resize()
{
	if (m_Size <= m_Capacity)
		return;

	m_Capacity *= 2;

	char* TempData = new char[m_Capacity];

	memcpy(TempData, m_WriteBuffer, m_Size - 1);
	delete m_WriteBuffer;

	m_WriteBuffer = TempData;
}