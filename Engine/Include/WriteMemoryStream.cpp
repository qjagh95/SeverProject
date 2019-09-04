#include "stdafx.h"
#include "WriteMemoryStream.h"

JEONG_USING

WriteMemoryStream::WriteMemoryStream()
{
}

WriteMemoryStream::~WriteMemoryStream()
{
}

void WriteMemoryStream::Write(const void * Data, size_t Length)
{
	char* TempData = (char*)Data;

	for (size_t i = 0; i < Length; i++)
	{
		m_WriteBuffer.push_back(TempData[i]);
	}
}

void WriteMemoryStream::HeaderErase()
{
	if (m_WriteBuffer.size() == 0)
		return;

	for (size_t i = 0; i < 4; i++)
		m_WriteBuffer.erase(m_WriteBuffer.begin() + i);
}

void WriteMemoryStream::BufferClear()
{
	m_WriteBuffer.clear();
}

void WriteMemoryStream::PullBuffer(size_t Size)
{
	if (m_WriteBuffer.size() == 0)
		return;
}