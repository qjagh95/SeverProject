#pragma once

JEONG_BEGIN

struct JEONG_DLL WinSize
{
	unsigned int Width;
	unsigned int Height;

	WinSize() : Width(0), Height(0) {}
	WinSize(const WinSize& size) {*this = size;}
	WinSize(unsigned int Width, unsigned Height) : Width(Width), Height(Height) {}
};

struct VertexColor
{
	Vector3 m_Pos;
	Vector4 m_Color;

	VertexColor() {}
	VertexColor(const VertexColor& Value) { *this = Value; }
	VertexColor(const Vector3& vec3, const Vector4& vec4) { m_Pos = vec3, m_Color = vec4; }
};

//UV좌표란 이미지크기에 상관없이 무조건 0에서 1까지의 범위를 갖는 숫자이다. (좌상단 좌표를 사용한다)
//Left = 0 , Right = 1
//Top = 0 , Bottom = 1
struct VertexUV
{
	Vector3 m_Pos;
	Vector2 m_UV;

	VertexUV() {}
	VertexUV(const VertexUV& Value) { *this = Value; }
	VertexUV(const Vector3& vec3, const Vector2& vec2) { m_Pos = vec3, m_UV = vec2; }
};

enum JEONG_DLL SHADER_TYPE
{
	ST_VERTEX,
	ST_PIXEL,
	ST_MAX,
};

//CBuffer은 상수버퍼 (C++코드에서 쉐이더로 보내기위함)
struct JEONG_DLL CBuffer
{
	ID3D11Buffer* cBuffer;
	int BufferSize;
	int ShaderType;
	int RegisterNumber;
};

struct JEONG_DLL MaterialCbuffer
{
	//재질정보 (색상) 나머지필요한 변수들은 나중에 추가 예정.
	Vector4 Diffuse;

	MaterialCbuffer() : Diffuse(Vector4::White) {}
};

struct JEONG_DLL BoxInfo
{
	Vector3 Min;
	Vector3 Max;
	Vector3 Lenth;
};

struct JEONG_DLL CircleInfo
{
	float Radius;
	Vector3 CenterPos;
	CircleInfo() { Radius = 0.0f; }
};

struct JEONG_DLL OBB2DInfo
{
	Vector3 CenterPos;
	Vector3 Axis[2];
	float Lenth[2];
};

//이걸 또 선언해주는 이유는 위에선언된 2DCBuffer를 void*로 넘겨주기위함이다.
//어차피 상수버퍼도 똑같은 바이트크기여서 위 내용 그대로 포인터로받아서 넘겨주는형식이다.
//왜나하면 map, unmap에서 memcpy후 쉐이더에 셋팅해주기때문.
struct JEONG_DLL RendererCBuffer
{
	void* pBuffer;
	string Name;
	int BufferSize;
};

//16
struct JEONG_DLL TransformCBuffer
{
	Matrix World;
	Matrix View;
	Matrix Projection;
	Matrix WV;
	Matrix WVP;
	Vector3 Pivot; //12
	float Empty1;	//4바이트
	Vector3 Lenth; //12
	float Empty2;   //4바이트 (16바이트패딩으로 바이트 맞춰주기위함)
};

//나중에 추가할 컴포넌트가 상수버퍼를 사용하는지 안하는지를 판단하는 상수버퍼.
struct JEONG_DLL ComponentCBuffer
{
	int TextureAnimation2D;
	Vector3 Empty;
};

struct SocketInfo
{
	SOCKET m_Socket;
	SOCKADDR_IN m_ClientInfo;
	size_t m_CliendID = 0;
};

struct IO_Data
{
	OVERLAPPED m_Overlapped;
	char m_Buffer[BUFFERSIZE] = {};
	WSABUF m_WsaBuf;

	IO_Data()
	{
		ZeroMemory(&m_Overlapped, sizeof(OVERLAPPED));
		m_WsaBuf.len = 0;
	}

	template<typename T>
	void WriteBuffer(char* Buffer)
	{
		ZeroMemory(m_Buffer, sizeof(m_WsaBuf.len));

		memcpy(m_Buffer, Buffer, sizeof(T));
		m_WsaBuf.len = sizeof(T);
		m_WsaBuf.buf = m_Buffer;
	}
	void WriteBuffer(char* Buffer, int Size)
	{
		ZeroMemory(m_Buffer, sizeof(m_WsaBuf.len));

		memcpy(m_Buffer, Buffer, Size);
		m_WsaBuf.len = Size;
		m_WsaBuf.buf = m_Buffer;
	}

	template<typename T>
	void WriteHeader(T& Message)
	{
		ZeroMemory(m_Buffer, sizeof(m_WsaBuf.len));

		memcpy(m_Buffer, &Message.m_Type, sizeof(T));
		m_WsaBuf.len = sizeof(T);
		m_WsaBuf.buf = m_Buffer;
	}

	//버퍼초기화
	template<typename T>
	void ClearBuffer()
	{
		ZeroMemory(&m_Buffer, sizeof(T));
		m_WsaBuf.buf = m_Buffer;
		m_WsaBuf.len = 0;
	}

	void PullBuffer(int Size)
	{
		memcpy(m_Buffer, m_Buffer + Size, Size);
	}

	template<typename T>
	void PullBuffer()
	{
		memcpy(m_Buffer, m_Buffer + sizeof(T), sizeof(T));
	}
};

JEONG_END
