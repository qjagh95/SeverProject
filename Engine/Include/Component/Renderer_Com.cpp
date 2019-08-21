#include "stdafx.h"
#include "Renderer_Com.h"
#include "Transform_Com.h"
#include "Camera_Com.h"
#include "Material_Com.h"

#include "../Device.h"
#include "../GameObject.h"

#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "../Render/Shader.h"
#include "../Render/RenderState.h"

JEONG_USING

Renderer_Com::Renderer_Com()
	:m_Mesh(NULLPTR), m_Shader(NULLPTR), m_LayOut(NULLPTR), m_Material(NULLPTR), m_isScreenRender(false)
{
	m_ComType = CT_RENDER;
	ZeroMemory(m_RenderState, sizeof(RenderState*) * RS_END);
	ZeroMemory(&m_ComponentCBuffer, sizeof(ComponentCBuffer));

	if (RenderManager::Get()->GetGameMode() == GM_2D)
		SetRenderState(DEPTH_DISABLE); //���⼭ 2D������Ʈ���� �̹� Set�� ���� ���� XX
}

Renderer_Com::Renderer_Com(const Renderer_Com& copyObject)
	:Component_Base(copyObject)
{
	*this = copyObject;

	if (m_Mesh != NULLPTR)
		m_Mesh->AddRefCount();

	if (m_Shader != NULLPTR)
		m_Shader->AddRefCount();

	for (int i = 0; i < RS_END; ++i)
	{
		if (m_RenderState[i] != NULLPTR)
			m_RenderState[i]->AddRefCount();
	}

	m_Material = NULLPTR;
	m_CBufferMap.clear();

	unordered_map<string, RendererCBuffer*>::const_iterator StartIter = copyObject.m_CBufferMap.begin();
	unordered_map<string, RendererCBuffer*>::const_iterator EndIter = copyObject.m_CBufferMap.end();

	for (; StartIter != EndIter; StartIter++)
	{
		RendererCBuffer* newCBuffer = new RendererCBuffer();
		newCBuffer->Name = StartIter->second->Name;
		newCBuffer->BufferSize = StartIter->second->BufferSize;
		newCBuffer->pBuffer = new char[newCBuffer->BufferSize];

		m_CBufferMap.insert(make_pair(StartIter->first, newCBuffer));
	}
}

Renderer_Com::~Renderer_Com()
{
	SAFE_RELEASE(m_Mesh);
	SAFE_RELEASE(m_Shader);
	SAFE_RELEASE(m_Material);

	for (int i = 0; i < RS_END; ++i)
		SAFE_RELEASE(m_RenderState[i]);

	unordered_map<string, RendererCBuffer*>::iterator StartIter = m_CBufferMap.begin();
	unordered_map<string, RendererCBuffer*>::iterator EndIter = m_CBufferMap.end();

	for (; StartIter != EndIter; StartIter++)
	{
		SAFE_DELETE_ARRARY(StartIter->second->pBuffer);
		SAFE_DELETE(StartIter->second);
	}

	m_CBufferMap.clear();
}

bool Renderer_Com::Init()
{
	//���� �������ִ� ������Ʈ�� AddComponent
	m_Material = AddComponent<Material_Com>("Material");
	CheckComponent();

	return true;
}

int Renderer_Com::Input(float DeltaTime)
{
	return 0;
}

int Renderer_Com::Update(float DeltaTime)
{
	return 0;
}

int Renderer_Com::LateUpdate(float DeltaTime)
{

	return 0;
}

void Renderer_Com::Collision(float DeltaTime)
{
}

void Renderer_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Renderer_Com::Render(float DeltaTime)
{
	if (m_Material == NULLPTR)
		m_Material = FindComponentFromType<Material_Com>(CT_MATERIAL);

	//������Ʈ �Լ����� �̹� World����� ��ȯ�� �Ǿ���.
	//������ ������Ʈ���� ������ȯ(Projection)���ϰ�, ���̴��� ���������� ������.(UpdateTransform)
	UpdateTransform();

	for (int i = 0; i < RS_END; i++)
	{
		if (m_RenderState[i] != NULLPTR)
			m_RenderState[i]->SetState();
	}

	////////////////////////////////////////////////////////////////////////////////////////////

	ShaderManager::Get()->UpdateCBuffer("Component", &m_ComponentCBuffer);
	//���ؽ� ������ ����� ���̴��� �������ִ� ����.
	Device::Get()->GetContext()->IASetInputLayout(m_LayOut);
	m_Shader->SetShader();

	for (size_t i = 0; i < m_Mesh->GetContainerCount(); i++)
	{
		for (size_t j = 0; j < m_Mesh->GetSubsetCount((int)i); j++)
		{
			m_Material->SetShader((int)i, (int)j);
			m_Mesh->Render((int)i, (int)j);
		}
	}

	for (int i = 0; i < RS_END; i++)
	{
		if (m_RenderState[i] != NULLPTR)
			m_RenderState[i]->ResetState();
	}
}

Renderer_Com * Renderer_Com::Clone()
{
	return new Renderer_Com(*this);
}

void Renderer_Com::AfterClone()
{
	m_Material = FindComponentFromType<Material_Com>(CT_MATERIAL);
}

void Renderer_Com::SetMesh(Mesh * mesh)
{
	SAFE_RELEASE(m_Mesh);
	m_Mesh = mesh;

	if(mesh != NULLPTR)
	{
		mesh->AddRefCount();

		SetShader(mesh->GetShaderKey());
		SetLayOut(mesh->GetLayOutKey());
	}
}

void Renderer_Com::SetMesh(const string & KeyName)
{
	SAFE_RELEASE(m_Mesh);

	//FindMesh���� �̹� Add����
	m_Mesh = ResourceManager::Get()->FindMesh(KeyName);

	if (m_Mesh != NULLPTR)
	{
		SetShader(m_Mesh->GetShaderKey());
		SetLayOut(m_Mesh->GetLayOutKey());
	}
}

void Renderer_Com::SetShader(Shader * shader)
{
	SAFE_RELEASE(m_Shader);
	m_Shader = shader;

	if (shader != NULLPTR)
		shader->AddRefCount();
}

void Renderer_Com::SetShader(const string & KeyName)
{
	SAFE_RELEASE(m_Shader);
	m_Shader = ShaderManager::Get()->FindShader(KeyName);
}

void Renderer_Com::SetLayOut(const string & KeyName)
{
	m_LayOut = ShaderManager::Get()->FindInputLayOut(KeyName);
}

void Renderer_Com::SetRenderState(const string & KeyName)
{
	RenderState* getState = RenderManager::Get()->FindRenderState(KeyName);

	if (getState == NULLPTR)
		return;

	//������� �迭�� ������ Ÿ�Ի����°�� ����ִ´�.
	m_RenderState[getState->GetStateEnum()] = getState;
}

bool Renderer_Com::CreateRendererCBuffer(const string & KeyName, int BufferSize)
{
	RendererCBuffer* newCBuffer = FindCBuffer(KeyName);

	if (newCBuffer != NULLPTR)
		return false;

	newCBuffer = new RendererCBuffer();
	newCBuffer->Name = KeyName;
	newCBuffer->pBuffer = new char[BufferSize];
	newCBuffer->BufferSize = BufferSize;

	m_CBufferMap.insert(make_pair(KeyName, newCBuffer));
	return true;
}

void Renderer_Com::UpdateRendererCBuffer(const string & KeyName, void * pData, int BufferSize)
{
	//���⼱ �޸� ���縸 �س��� �������� ���̴��� �ѱ��.
	RendererCBuffer* getCBuffer = FindCBuffer(KeyName);

	if (getCBuffer == NULLPTR)
	{
		getCBuffer = new RendererCBuffer();
		getCBuffer->Name = KeyName;
		getCBuffer->pBuffer = new char[BufferSize];
		getCBuffer->BufferSize = BufferSize;

		m_CBufferMap.insert(make_pair(KeyName, getCBuffer));
	}
	memcpy(getCBuffer->pBuffer, pData, BufferSize);
}

//���⿡�� �������� ���������� ��ȯ�� ���ش�!
void Renderer_Com::UpdateTransform()
{
	//m_Transform�� GameObject�� �����ɶ� �����Ҵ����� �ڵ������Ǹ� �ڵ��ʱ�ȭ�� �Ѵ�.
	//AddComponent�� �Ҷ� Component�� ���� Transform_Com������ ������Ʈ�� ���� Transform_Com
	//������ �̹� �ʱ�ȭ�� �Ǿ��ִ�.

	TransformCBuffer cBuffer = {};
	Camera_Com* getCamera = NULLPTR;

	if (m_isScreenRender == true)
		getCamera = m_Scene->GetUICamera();
	else
		getCamera = m_Scene->GetMainCamera();
	
	cBuffer.World = m_Transform->GetWorldMatrix();
	cBuffer.View = getCamera->GetViewMatrix();
	cBuffer.Projection = getCamera->GetProjection();

	cBuffer.WV = cBuffer.World * cBuffer.View;
	cBuffer.WVP = cBuffer.WV * cBuffer.Projection;

	cBuffer.Pivot = m_Transform->GetPivot();
	cBuffer.Lenth = m_Mesh->GetLenth();

	//���̴��� �����ֱ����Ͽ� ��ġ��ķ� ��ȯ�Ѵ�.
	cBuffer.World.Transpose();
	cBuffer.View.Transpose();
	cBuffer.Projection.Transpose();
	cBuffer.WV.Transpose();
	cBuffer.WVP.Transpose();

	//�̸� �����س��� �������(ID3D11Buffer)�� �����ͼ� ������Ʈ ��Ų��.
	//���ؽ����̴��� �ȼ����̴��� ������ ���� ������ ��ĵ��� (�������) �������ش�. 
	ShaderManager::Get()->UpdateCBuffer("Transform", &cBuffer);
}

RendererCBuffer * Renderer_Com::FindCBuffer(const string & KeyName)
{
	unordered_map<string, RendererCBuffer*>::iterator FindIter = m_CBufferMap.find(KeyName);

	if (FindIter == m_CBufferMap.end())
		return NULLPTR;

	return FindIter->second;
}

void Renderer_Com::CheckComponent()
{
	const list<Component_Base*>* TempList = m_Object->GetComponentList();

	list<Component_Base*>::const_iterator StartIter = TempList->begin();
	list<Component_Base*>::const_iterator EndIter = TempList->end();

	for (;StartIter != EndIter ;StartIter++)
	{
		switch ((*StartIter)->GetComType())
		{
			case CT_ANIMATION2D:
				m_ComponentCBuffer.TextureAnimation2D = 1;
				break;
		}
	}
}

void Renderer_Com::DeleteComponentCBuffer(Component_Base * DeleteCom)
{
	switch (DeleteCom->GetComType())
	{
		case CT_ANIMATION2D:
			m_ComponentCBuffer.TextureAnimation2D = 1;
			break;
	}
}
	