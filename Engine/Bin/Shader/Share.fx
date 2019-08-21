//���̴������� �߰��� ������ �����ϴ�.

//������� ����̹����� �׳� ����ϴٰ� ���ڱ� �ǰݽ� ���� ������� �ٲ����Ѵٸ� �������̴��� ���������� ü���� �� 
//�ٽ� �⺻���̴��� ���ƿ��Ⱑ ������ (9������ ����)

//���� �ڿ� : (�ø�ƽ)

//���̴��� 1��¥���ڵ�� ������ŭ ����Ǳ⶧���� ����¥���� �ƴϴ�
//���ؽ��� 2������� ����¥���ڵ尡 2����¥�� �ڵ尡�ȴ�.

struct VS_INPUT_COLOR
{
    //�ø�ƽ�� �̸� �ڿ� ���ڸ� �ٿ��ش�. �Ⱥ��ϰ�� �⺻���� 0
    //�����̸� ������ : ��Ʈ�ʵ�(��Ʈ��)
    float3 vPos : POSITION; 
    float4 vColor : COLOR;
};

struct VS_OUTPUT_COLOR
{
    //SV = SystemValue -> �ٲ� �� ����.
    float4 vPos : SV_POSITION;
    float4 vColor : COLOR;
};

/////////////////////////////////////////////////////////////////////
struct VS_INPUT_POS
{
    float3 vPos : POSITION; 
};

struct VS_OUTPUT_POS
{
    float4 vPos : SV_POSITION;
};

/////////////////////////////////////////////////////////////////////
struct VS_INPUT_UV
{
    float3 vPos : POSITION; //����Ʈ Pos
    float2 vUV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
};

struct PS_OUTPUT_SINGLE
{
    float4 vTarget0 : SV_Target;
};

static const float2 NULLPos[4] =
{
    float2(-1.0f, 1.0f),
	float2(1.0f, 1.0f),
	float2(-1.0f, -1.0f),
	float2(1.0f, -1.0f)
};

static const float2 NULLUV[4] =
{
    float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 1.0f)
};


/////////////////////////////////////////////////////////////////////

//CBuffer�� ������� (C++�ڵ忡�� ���̴��� ���� �޾ƿ��� ����)
//cbuffer �Լ��� : �������Ϳ� b0�� �����ϰڴ�.
cbuffer Transform : register(b0)
{
    matrix g_World;       //�������
    matrix g_View;        //�����
    matrix g_Projection;  //�������

    //�ΰ��� �� �����ϴ� ������ ���ؽ����� ���ϴ� �� ����
    //�̹� ������ ���� ���ͼ� �����ϴ°� �ξ��� �����⶧���̴�.
    matrix g_WV;    //World * View
    matrix g_WVP;   //World * View * Projection

    float3 g_Pivot;
    float g_Empty1;
    float3 g_Length;
    float g_Empty2;
}

cbuffer Material : register(b1)
{
    float4 g_MaterialDiffuse;
}

cbuffer Componeent : register(b2)
{
    int g_Animation2DEnable;
    float3 g_Empty;
}

//��������
/////////////////////////////////////////////////////////////////////

Texture2D Diffuse : register(t0);
SamplerState DiffuseSampler : register(s0);
Texture2D TargetDiffuse : register(t10);

/////////////////////////////////////////////////////////////////////