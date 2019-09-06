#pragma once

JEONG_BEGIN

class JEONG_DLL WriteMemoryStream
{
public:
	void Write(const void* Data, size_t Length);
	size_t GetSize() const { return m_Size; }
	char* GetBuffer();
	void HeaderErase();
	void BufferClear();
	void PullBuffer(size_t Size);
	
private:
	void Resize();

public:
	size_t m_Size;
	size_t m_Capacity;
	char* m_Buffer;

public:
	WriteMemoryStream();
	~WriteMemoryStream();
};

JEONG_END
