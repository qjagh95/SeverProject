#pragma once

JEONG_BEGIN

class JEONG_DLL ReadMemoryStream
{
public:
	char* GetBuffer() const { return m_ReadBuffer; }
	size_t GetSize() const { return m_Size; }
	void Read(void* Buffer, size_t length);

private:
	char* m_ReadBuffer;
	size_t m_Size;
	size_t m_Capacity;
	bool m_isInit;

public:
	ReadMemoryStream(char* InputBuffer);
	~ReadMemoryStream();
};

JEONG_END