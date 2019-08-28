#include "stdafx.h"
#include "WriteMemoryStream.h"

JEONG_USING

WriteMemoryStream::WriteMemoryStream()
{
	m_Capacity = 0;
	m_Size = 0;

	m_isInit = false;
	m_WriteBuffer = nullptr;
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

void WriteMemoryStream::Resize()
{
	if (m_Size < m_Capacity)
		return;

	m_Capacity *= 2;

	char* TempData = new char[m_Capacity];
	memcpy(TempData, m_WriteBuffer, m_Size);
	delete m_WriteBuffer;

	m_WriteBuffer = TempData;
}
