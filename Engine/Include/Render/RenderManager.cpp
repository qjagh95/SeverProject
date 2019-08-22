#include "stdafx.h"
#include "RenderManager.h"
#include "RenderTarget.h"

JEONG_USING
SINGLETON_VAR_INIT(RenderManager)

RenderManager::RenderManager()
	:m_CreateState(NULLPTR)
{
	m_GameMode = GM_2D;
	m_isZoomMode = false;
}

RenderManager::~RenderManager()
{
	ShaderManager::Delete();
	Safe_Release_Map(m_RenderStateMap);
	
	unordered_map<string, RenderTarget*>::iterator StartIter = m_RenderTargetMap.begin();
	unordered_map<string, RenderTarget*>::iterator EndIter = m_RenderTargetMap.end();

	for (; StartIter != EndIter; StartIter++)
		SAFE_DELETE(StartIter->second);

	m_RenderTargetMap.clear();

	for (int i = 0; i < RG_END; ++i)
	{
		for (int j = 0; j < m_RenderGroup[i].Size; ++j)
		{
			SAFE_RELEASE(m_RenderGroup[i].ObjectList[j]);
		}
		m_RenderGroup[i].Size = 0;
	}
}

bool RenderManager::Init()
{
	if (ShaderManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	AddBlendTargetDesc(true);
	CreateBlendState(ALPHA_BLEND);
	CreateDepthStencilState(DEPTH_DISABLE, FALSE);

	if (CreateRenderTarget("PostEffect", DXGI_FORMAT_R8G8B8A8_UNORM, Vector3::Zero, Vector3(0.0f, 0.0f, 1.0f), true, Vector4::Black) == false)
	{
		TrueAssert(true);
		return false;
	}

	return true;
}

void RenderManager::AddBlendTargetDesc(BOOL bEnable, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphaBlend, D3D11_BLEND destAlphaBlend, D3D11_BLEND_OP blendAlphaOp, UINT8 iWriteMask)
{
	if (m_CreateState == NULLPTR)
		m_CreateState = new BlendState();

	m_CreateState->AddTargetDesc(bEnable, srcBlend, destBlend,blendOp, srcAlphaBlend, destAlphaBlend, blendAlphaOp,iWriteMask);
}

bool RenderManager::CreateDepthStencilState(const string & KeyName, BOOL bDepthEnable, D3D11_DEPTH_WRITE_MASK eMask, D3D11_COMPARISON_FUNC eDepthFunc, BOOL bStencilEnable, UINT8 iStencilReadMask, UINT8 iStencilWriteMask, D3D11_DEPTH_STENCILOP_DESC tFrontFace, D3D11_DEPTH_STENCILOP_DESC tBackFace)
{
	DepthStancilState* newState = (DepthStancilState*)FindRenderState(KeyName);

	if (newState != NULLPTR)
		return false;

	newState = new DepthStancilState();

	if (newState->CreateState(bDepthEnable, eMask, eDepthFunc, bStencilEnable, iStencilReadMask, iStencilWriteMask, tFrontFace, tBackFace) == false)
	{
		SAFE_RELEASE(newState);
		return false;
	}

	m_RenderStateMap.insert(make_pair(KeyName, newState));

	return true;
}

bool RenderManager::CreateRenderTarget(const string & KeyName, DXGI_FORMAT TargetFormat, const Vector3 & Pos, const Vector3 & Scale, bool isDebugDraw, const Vector4 & ClearColor, DXGI_FORMAT DepthFormat)
{
	RenderTarget* newTarget = FindRenderTarget(KeyName);

	if (newTarget != NULLPTR)
		return false;

	newTarget = new RenderTarget();

	if (newTarget->CreateRenderTarget(TargetFormat, Pos, Scale, DepthFormat) == false)
	{
		SAFE_DELETE(newTarget);
		return false;
	}

	newTarget->SetClearColor(ClearColor);
	newTarget->SetDrawDebug(isDebugDraw);

	m_RenderTargetMap.insert(make_pair(KeyName, newTarget));

	return true;
}

bool RenderManager::CreateBlendState(const string & KeyName, BOOL bAlphaCoverage, BOOL bIndependent)
{
	if (m_CreateState == NULLPTR)
		return false;

	if (m_CreateState->CreateState(bAlphaCoverage, bIndependent) == false)
	{
		SAFE_RELEASE(m_CreateState);
		return false;
	}

	m_RenderStateMap.insert(make_pair(KeyName, m_CreateState));
	m_CreateState = NULLPTR;

	return true;
}

RenderState * RenderManager::FindRenderState(const string & KeyName)
{
	unordered_map<string, RenderState*>::iterator FindIter = m_RenderStateMap.find(KeyName);

	if (FindIter == m_RenderStateMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}

RenderTarget * RenderManager::FindRenderTarget(const string & KeyName)
{
	unordered_map<string, RenderTarget*>::iterator FindIter = m_RenderTargetMap.find(KeyName);

	if (FindIter == m_RenderTargetMap.end())
		return NULLPTR;

	return FindIter->second;
}

void RenderManager::AddRenderObject(GameObject * object)
{
	RENDER_GROUP group = RG_NORMAL;

	if (object->CheckComponentType(CT_STAGE2D))
		group = RG_LANDSCAPE;

	else if (object->CheckComponentType(CT_UI))
		group = RG_UI;

	if (m_RenderGroup[group].Size == m_RenderGroup[group].Capacity)
	{
		m_RenderGroup[group].Capacity *= 2;

		GameObject** newObject = new GameObject*[m_RenderGroup[group].Capacity];
		{
			memcpy(newObject, m_RenderGroup[group].ObjectList, sizeof(GameObject*) * m_RenderGroup[group].Size);
		}
		SAFE_DELETE_ARRARY(m_RenderGroup[group].ObjectList);

		m_RenderGroup[group].ObjectList = newObject;
	}

	m_RenderGroup[group].ObjectList[m_RenderGroup[group].Size] = object;
	m_RenderGroup[group].Size++;
}

void RenderManager::Render(float DeltaTime)
{
	Render2D(DeltaTime);
}

void RenderManager::Render2D(float DeltaTime)
{
	// ����Ʈ ����Ʈ ó���� Ÿ������ ��ü�Ѵ�.
	RenderTarget* getTarget = FindRenderTarget("PostEffect");
	getTarget->ClearTarget();
	getTarget->SetTarget();
	{
		//���ı׷���� ���.
		for (int i = 0; i <= RG_ALPHA3; ++i)
		{
			for (int j = 0; j < m_RenderGroup[i].Size; ++j)
			{
				m_RenderGroup[i].ObjectList[j]->Render(DeltaTime);
			}
			m_RenderGroup[i].Size = 0;
		}
	}
	getTarget->ResetTarget();

	// ���⿡�� ����Ʈ ����Ʈ�� ó���Ѵ�.
	// ���⿡�� ����Ʈ����Ʈ ó���� �� Ÿ���� ��ü ũ��� ȭ�鿡 ����Ѵ�.
	getTarget->RenderFullScreen();

	// UI����~���
	for (int i = RG_UI; i < RG_END; ++i)
	{
		for (int j = 0; j < m_RenderGroup[i].Size; ++j)
		{
			m_RenderGroup[i].ObjectList[j]->Render(DeltaTime);
		}
		m_RenderGroup[i].Size = 0;
	}
	//Ÿ���� ���ĺ��� ó�����ְ� ���� �������.
	RenderState* alphaState = FindRenderState(ALPHA_BLEND);
	alphaState->SetState();
	{
		unordered_map<string, RenderTarget*>::iterator StartIter = m_RenderTargetMap.begin();
		unordered_map<string, RenderTarget*>::iterator EndIter = m_RenderTargetMap.end();

		for (; StartIter != EndIter; StartIter++)
		{
			StartIter->second->Render(DeltaTime);
		}
	}
	alphaState->ResetState();
	SAFE_RELEASE(alphaState);
}
