#pragma once
#include "../RefCount.h"

JEONG_BEGIN

class JEONG_DLL RenderState : public RefCount
{
public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;

	RENDER_STATE GetStateEnum() const { return m_State; }

protected:
	//ID3D11DeviceChild = BlendState, DepthCilState, ResterizerState�� �ֻ��� �θ�Ŭ������.
	ID3D11DeviceChild* m_RenderState;
	ID3D11DeviceChild* m_OldRenderState;

	RENDER_STATE m_State;

protected:
	RenderState();
	virtual ~RenderState() = 0;

public:
	friend class RenderManager;
};
JEONG_END
