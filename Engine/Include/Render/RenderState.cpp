#include "stdafx.h"
#include "RenderState.h"

JEONG_USING

RenderState::RenderState()
	:m_RenderState(NULLPTR), m_OldRenderState(NULLPTR)
{
}

RenderState::~RenderState()
{
	SAFE_RELEASE(m_RenderState);
	SAFE_RELEASE(m_OldRenderState);
}
