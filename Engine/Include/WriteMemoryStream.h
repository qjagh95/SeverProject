#pragma once
class WriteMemoryStream
{
public:
	void Write(const void* Data, size_t Length);
	size_t GetSize() const { return m_Size; }
	size_t GetCapacity() const { return m_Capacity; }
	char* GetBuffer() const { return m_MemoryData; }

private:
	void Resize();

private:
	char* m_MemoryData;
	size_t m_Size;
	size_t m_Capacity;
	bool m_isInit;

public:
	WriteMemoryStream();
	~WriteMemoryStream();
};

