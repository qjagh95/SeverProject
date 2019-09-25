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
	//메세지헤더(4바이트)뒤에 내사이즈를 넣는다
	void InsertByteSize();
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
