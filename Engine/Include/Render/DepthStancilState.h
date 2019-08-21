#pragma once
#include "RenderState.h"

JEONG_BEGIN

//��Ŭ������ 2D 3D��忡���� ��� ������Ʈ�鿡�� �ϳ��� ���̹��۸� ����ϰų�, ������� �������� 
//����̽��� �˸������Ѱ��̴�.
class DepthStancilState : public RenderState
{
public:
	bool CreateState(BOOL DepthEnable, D3D11_DEPTH_WRITE_MASK Mask = D3D11_DEPTH_WRITE_MASK_ALL , D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS, BOOL StencilEnable = FALSE, UINT8 StencilReadMask = 0, UINT8 StencilWriteMask = 0, D3D11_DEPTH_STENCILOP_DESC FrontFace = {}, D3D11_DEPTH_STENCILOP_DESC BackFace = {});

	void SetState() override;
	void ResetState() override;

private:
	UINT m_StencilRef;
	UINT m_OldStencilRef;

private:
	DepthStancilState();
	~DepthStancilState();

public:
	friend class RenderManager;
};

JEONG_END