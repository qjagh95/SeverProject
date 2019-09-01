#pragma once

JEONG_BEGIN

class JEONG_DLL WriteMemoryStream
{
public:
	void Write(const void* Data, size_t Length);
	size_t GetSize() const { return m_Size; }
	size_t GetCapacity() const { return m_Capacity; }
	char* GetBuffer() { return m_WriteBuffer; }
	void HeaderErase();
	void BufferClear();

private:
	void Resize();

public:
	char* m_WriteBuffer;

private:
	size_t m_Size;
	size_t m_Capacity;
	bool m_isInit;

public:
	WriteMemoryStream();
	~WriteMemoryStream();
};

JEONG_END
