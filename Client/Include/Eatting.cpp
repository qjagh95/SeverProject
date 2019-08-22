#include "ClientHeader.h"
#include "Eatting.h"
#include "Stage_Com.h"
#include "UserComponent/Player_Com.h"

#include <Component/ColliderRect_Com.h>

Eatting::Eatting()
{
	m_isUpdate = true;
	m_IncludeStage = NULLPTR;
	m_Index = 0;
}

Eatting::~Eatting()
{
}

bool Eatting::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("PlayerRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

	BasicInfo::Init();

	m_Material->SetDiffuseTexture(0, "Circle", TEXT("Circle.png"));

	m_CirCleColl = m_Object->AddComponent<JEONG::ColliderCircle_Com>("EattingCircle");
	m_CirCleColl->SetCollsionCallback(CCT_FIRST, this, &Eatting::PlayerColl);

	return true;
}

int Eatting::Input(float DeltaTime)
{
	return 0;
}

int Eatting::Update(float DeltaTime)
{
	return 0;
}

int Eatting::LateUpdate(float DeltaTime)
{
	return 0;
}

void Eatting::Collision(float DeltaTime)
{                                            
}

void Eatting::Render(float DeltaTime)
{
}

Eatting * Eatting::Clone()
{
	return nullptr;
}

void Eatting::PlayerColl(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "PlayerCircle")
	{
		//서버로 파괴될 오브젝트 정보로 나자신 넘기기(서버가 Delete명령, 명령에따른 Delete호출)
		Player_Com* getPlayer = Dest->FindComponentFromType<Player_Com>(static_cast<COMPONENT_TYPE>(UT_PLAYER));
		getPlayer->ScalePlus(m_Scale);

		SAFE_RELEASE(getPlayer);
	}
}