#include "ClientHeader.h"
#include "BasicInfo.h"

BasicInfo::BasicInfo()
{
	m_Scale = 0.0f;
	m_RGB = Vector3::Zero;

	m_CirleColl = nullptr;
	m_RectColl = nullptr;
	m_CollVar = 0.0f;
}

BasicInfo::~BasicInfo()
{
}

bool BasicInfo::Init()
{
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

void BasicInfo::CollisionLateUpdate(float DeltaTime)
{
}

void BasicInfo::Render(float DeltaTime)
{
}

BasicInfo * BasicInfo::Clone()
{
	return nullptr;
}

void BasicInfo::SendPos()
{
}

void BasicInfo::SendScale()
{
}

void BasicInfo::RecvPos()
{
}

void BasicInfo::RecvRGB()
{
}

void BasicInfo::RecvScale()
{
}


void BasicInfo::SetRGB(float R, float G, float B)
{
	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetMaterial(Vector4(R / 255.0f, G / 255.0f, B / 255.0f, 1.0f));
	SAFE_RELEASE(MaterialComponent);
}

void BasicInfo::SetScale(float Scale)
{
	m_Scale = Scale;
	m_Transform->SetWorldScale(m_Transform->GetWorldScale() + m_Scale);
}
