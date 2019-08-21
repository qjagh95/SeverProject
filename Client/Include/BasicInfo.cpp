#include "ClientHeader.h"
#include "BasicInfo.h"

BasicInfo::BasicInfo()
{
}


BasicInfo::~BasicInfo()
{
}

bool BasicInfo::Init()
{
	m_Scale = 0.0f;
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
