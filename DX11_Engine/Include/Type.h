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
};

struct JEONG_DLL OBB2DInfo
{
	Vector3 CenterPos;
	Vector3 Axis[2];
	float Lenth[2];
};

struct JEONG_DLL Pixel24
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct JEONG_DLL Pixel32
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct JEONG_DLL PixelInfo
{
	BoxInfo ImageRect;
	Pixel24 *Color;
	Pixel24 OutColor;
	int Width;
	int Height;
};

struct JEONG_DLL Clip2DFrame
{
	Vector2	LeftTop;
	Vector2	RightBottom;
};

struct JEONG_DLL AnimationClip2D
{
	ANIMATION2D_TYPE AnimationType;		///�̹�������(��Ʋ��, ������)
	ANIMATION_OPTION AnimationOption;	///��������
	string AnimationName;				///���� �� �ִϸ��̼� �̸�
	class Texture* CurTexture;			///���� �������ִ� �ؽ���
	float TextureWidth;					///�̹���ũ��
	float TextureHeight;				
	vector<Clip2DFrame>	vecFrame;		///�̹�����ġ(��������ġ)
	int Frame;							///����������
	float PlayTime;						///�ִϸ��̼� ���� �ð�
	float PlayLimitTime;				///��� �ִϸ��̼� �Ϸ� �ð�.
};

//���̴����� �̹����� �ɰ��� �ѷ��ٰ��̱⶧���� ���̴��� �Ѱ��� ������� ����
struct JEONG_DLL Animation2DCBuffer
{
	Vector2 LeftTopUV;
	Vector2 RightBottomUV; 
	int Frame;
	Vector3 Empty;
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

struct JEONG_DLL ButtonCBuffer
{
	Vector4 DiffuseColor;
};

struct JEONG_DLL CheckBoxCBuffer
{
	int isCheck;
	Vector3 Empty;
	Vector4 CheckBoxColor;
};

struct JEONG_DLL BarCBuffer
{
	Vector4 Light;
};

JEONG_END
