#include "ClientHeader.h"
#include "Stage_Com.h"
#include "Eatting.h"

Stage_Com::Stage_Com()
{
}

Stage_Com::Stage_Com(const Stage_Com & CopyData)
	:UserComponent_Base(CopyData)
{
}

Stage_Com::~Stage_Com()
{
}

bool Stage_Com::Init()
{
	return true;
}

int Stage_Com::Input(float DeltaTime)
{
	return 0;
}

int Stage_Com::Update(float DeltaTime)
{
	return 0;
}

int Stage_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Stage_Com::Collision(float DeltaTime)
{
}

void Stage_Com::Render(float DeltaTime)
{
}

Stage_Com * Stage_Com::Clone()
{
	return nullptr;
}

void Stage_Com::CreateEatting(const Vector3 & Pos)
{
}
