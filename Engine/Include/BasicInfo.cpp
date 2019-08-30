#include "stdafx.h"
#include "BasicInfo.h"

#include "Component/ColliderCircle_Com.h"
#include "Component/ColliderRect_Com.h"

JEONG_USING

BasicInfo::BasicInfo()
{
	m_Scale = 0.0f;
	m_CollScale = 0.0f;
	m_RGB = Vector4::Zero;

	m_CirCleColl = NULLPTR;
	m_RectColl = NULLPTR;
	m_Material = NULLPTR;
	m_CollVar = 0.0f;
}

BasicInfo::~BasicInfo()
{
	SAFE_RELEASE(m_CirCleColl);
	SAFE_RELEASE(m_RectColl);
	SAFE_RELEASE(m_Material);
}

bool BasicInfo::Init()
{
	m_Material = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);

	return true;
}

int BasicInfo::Input(float DeltaTime)
{
	return 0;
}

int BasicInfo::Update(float DeltaTime)
{
	return 0;
}

int BasicInfo::LateUpdate(float DeltaTime)
{
	return 0;
}

void BasicInfo::Collision(float DeltaTime)
{
}

void BasicInfo::Render(float DeltaTime)
{
}

BasicInfo * BasicInfo::Clone()
{
	return nullptr;
}

void BasicInfo::SetRGB(float R, float G, float B)
{
	m_RGB = Vector4(R, G, B, 1.0f);
	
	if (m_Material == NULLPTR)
	{
		m_Material = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
		m_Material->SetMaterial(Vector4(m_RGB.x, m_RGB.y, m_RGB.z, 1.0f));
		return;
	}

	m_Material->SetMaterial(m_RGB);
}

void BasicInfo::SetScale(float Scale)
{
	m_Scale = Scale;
	m_CollScale = Scale * 0.5f;

	m_Transform->SetWorldScale(m_Scale);
	m_CirCleColl->AddInfo(m_CollScale);
}

void BasicInfo::SetCollScale(float Scale)
{
	m_Scale = Scale;
	m_CollScale = Scale * 0.5f;

	m_Transform->SetWorldScale(Scale);
	m_CirCleColl->SetInfo(m_CollScale);
}
