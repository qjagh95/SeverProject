#pragma once

JEONG_BEGIN

class JEONG_DLL WriteMemoryStream
{
public:
	void Write(const void* Data, size_t Length);
	size_t GetSize() const { return m_Size; }
	size_t GetCapacity() const { return m_Capacity; }
	char* GetBuffer() const { return m_WriteBuffer; }

private:
	void Resize();

private:
	char* m_WriteBuffer;
	size_t m_Size;
	size_t m_Capacity;
	bool m_isInit;

public:
	WriteMemoryStream();
	~WriteMemoryStream();
};

JEONG_END
