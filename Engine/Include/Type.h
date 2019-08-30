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

//UV��ǥ�� �̹���ũ�⿡ ������� ������ 0���� 1������ ������ ���� �����̴�. (�»�� ��ǥ�� ����Ѵ�)
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

//CBuffer�� ������� (C++�ڵ忡�� ���̴��� ����������)
struct JEONG_DLL CBuffer
{
	ID3D11Buffer* cBuffer;
	int BufferSize;
	int ShaderType;
	int RegisterNumber;
};

struct JEONG_DLL MaterialCbuffer
{
	//�������� (����) �������ʿ��� �������� ���߿� �߰� ����.
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

//�̰� �� �������ִ� ������ ��������� 2DCBuffer�� void*�� �Ѱ��ֱ������̴�.
//������ ������۵� �Ȱ��� ����Ʈũ�⿩�� �� ���� �״�� �����ͷι޾Ƽ� �Ѱ��ִ������̴�.
//�ֳ��ϸ� map, unmap���� memcpy�� ���̴��� �������ֱ⶧��.
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
	float Empty1;	//4����Ʈ
	Vector3 Lenth; //12
	float Empty2;   //4����Ʈ (16����Ʈ�е����� ����Ʈ �����ֱ�����)
};

//���߿� �߰��� ������Ʈ�� ������۸� ����ϴ��� ���ϴ����� �Ǵ��ϴ� �������.
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

	//�����ʱ�ȭ
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
