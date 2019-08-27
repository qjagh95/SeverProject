#include "Share.fx"
//fx������ ���ؽ��� �ȼ����̴� �ΰ� ���ÿ� ó���� �����ϴ�.

cbuffer Animation2D : register(b8)
{
    float2 g_LeftTopUV;
    float2 g_RightBottomUV;
    int g_Frame;
    float3 g_Empty3;
}

//////////////////////////////////ColorShader//////////////////////////

VS_OUTPUT_COLOR Standard_Color_VS(VS_INPUT_COLOR input)
{
    //0���� �ʱ�ȭ
    VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR)0;
    //mul = ���ϱ� (������İ�)
    output.vPos = mul(float4(input.vPos, 1.0f), g_WVP);
    output.vColor = input.vColor;

    return output;
}

//���ؽ� ���̴����� ���� OutPut�����Ͱ� �ȼ����̴� Input���� ���´�. (�Ž�Ŭ���� �������� �̹� �� ������ �س���(IA~) )
PS_OUTPUT_SINGLE Standard_Color_PS(VS_OUTPUT_COLOR input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

    output.vTarget0 = g_MaterialDiffuse;

    return output;
}

//////////////////////////////////ColorShader//////////////////////////


//////////////////////////////////UVShader/////////////////////////////

VS_OUTPUT_UV Standard_UV_VS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;

    //API�� �ߴ� �߽������� (newPos = Pos - Size * Pivot)�̰Ŷ� �Ȱ���.
    //���ؽ��� ����������ȯ �� ��ǥ�� 0.n���¿��� �߽����� ��´�.
    float3 TempPos = input.vPos - (g_Pivot * g_Length);

    output.vPos = mul(float4(TempPos, 1.0f), g_WVP);

    //�ִϸ��̼��� �ִٸ� UV���� �� ��� (UV���� CPU���� �� ���´�)
    //���ٸ� �׳� ���
    if (g_Animation2DEnable == 1)
    {
        if (input.vUV.x == 0.0f)
            output.vUV.x = g_LeftTopUV.x;
        else
            output.vUV.x = g_RightBottomUV.x;

        if (input.vUV.y == 0.0f)
            output.vUV.y = g_LeftTopUV.y;
        else
            output.vUV.y = g_RightBottomUV.y;
    }
    else
        output.vUV = input.vUV;

    return output;
}
  
PS_OUTPUT_SINGLE Standard_UV_PS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;
    
    //Diffuse(Texture2D)�� SampleState(���������� UV)�� �־��ְ� ���������� ���Ѵ�
    output.vTarget0 = Diffuse.Sample(DiffuseSampler, input.vUV) * g_MaterialDiffuse;

    return output;
}
//////////////////////////////////UVShader/////////////////////////////

//////////////////////////////////STAITC Shader/////////////////////////////

VS_OUTPUT_UV StandardTexStaticVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float3 TempPos = input.vPos - g_Pivot * g_Length;

    output.vPos = mul(float4(TempPos, 1.0f), g_WVP);
    output.vUV = input.vUV;

    return output;
}

PS_OUTPUT_SINGLE StandardTexStaticPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = Diffuse.Sample(DiffuseSampler, input.vUV);

    return output;
}

//////////////////////////////////NULLBuffer��¿�/////////////////////////////

VS_OUTPUT_UV FullScreenVS(uint VertexID : SV_VertexID)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    output.vPos = float4(NULLPos[VertexID], 0.0f, 1.0f);
    output.vUV = NULLUV[VertexID];

    return output;
}

PS_OUTPUT_SINGLE FullScreenPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = Diffuse.Sample(DiffuseSampler, input.vUV);
    return output;
}