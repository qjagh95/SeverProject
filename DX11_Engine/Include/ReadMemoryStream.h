#pragma once

JEONG_BEGIN

class JEONG_DLL ReadMemoryStream
{
public:
	char* GetBuffer() const { return m_ReadBuffer; }
	size_t GetSize() const { return m_BufferSize; }

	template<typename T>
	T Read()
	{
		if (m_BufferSize == 0)
			return (T)0;

		T Temp;

		memcpy(&Temp, m_ReadBuffer, sizeof(T));
		memcpy(m_ReadBuffer, m_ReadBuffer + sizeof(T), m_BufferSize - sizeof(T));

		return Temp;
	}

private:
	char* m_ReadBuffer;
	size_t m_BufferSize;

public:
	ReadMemoryStream(char* InputBuffer, size_t BufferSize);
	~ReadMemoryStream();
};

JEONG_END