#pragma once

JEONG_BEGIN

class JEONG_DLL WriteMemoryStream
{
public:
	void Write(const void* Data, size_t Length);
	size_t GetSize() const { return m_WriteBuffer.size(); }
	char* GetBuffer() { return &m_WriteBuffer[0]; }
	void HeaderErase();
	void BufferClear();
	void PullBuffer(size_t Size);

public:
	vector<char> m_WriteBuffer;

public:
	WriteMemoryStream();
	~WriteMemoryStream();
};

JEONG_END
