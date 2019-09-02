#pragma once

JEONG_BEGIN

class JEONG_DLL WriteMemoryStream
{
public:
	void Write(const void* Data, size_t Length);
	size_t GetSize() const { return m_Size; }
	size_t GetCapacity() const { return m_Capacity; }
	char* GetBuffer() { return m_WriteBuffer; }
	void SetSize(int Size) { m_Size = Size; }
	void HeaderErase();
	void BufferClear();
	void PullBuffer(size_t Size);

	char* ReadBuffer(size_t Size);

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
