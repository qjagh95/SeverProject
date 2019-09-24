#include "ClientHeader.h"
#include "Eatting_Com.h"
#include "Stage_Com.h"
#include "Player_Com.h"

Eatting_Com::Eatting_Com()
{
	m_isUpdate = true;
	m_IncludeStage = NULLPTR;
	m_Index = 0;
}

Eatting_Com::~Eatting_Com()
{
}

bool Eatting_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("PlayerRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

	BasicInfo::Init();

	m_Material->SetDiffuseTexture(0, "Circle", TEXT("Circle.png"));

	m_CirCleColl = m_Object->AddComponent<ColliderCircle_Com>("EattingCircle");
	m_CirCleColl->SetCollsionCallback(CCT_FIRST, this, &Eatting_Com::PlayerColl);

	return true;
}

int Eatting_Com::Input(float DeltaTime)
{
	return 0;
}

int Eatting_Com::Update(float DeltaTime)
{
	return 0;
}

int Eatting_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Eatting_Com::Collision(float DeltaTime)
{                                            
}

void Eatting_Com::Render(float DeltaTime)
{
}

Eatting_Com * Eatting_Com::Clone()
{
	return nullptr;
}

void Eatting_Com::PlayerColl(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "PlayerCircle")
	{
		//서버로 파괴될 오브젝트 정보로 나자신 넘기기(서버가 Delete명령, 명령에따른 Delete호출)
		Player_Com* getPlayer = Dest->FindComponentFromType<Player_Com>(CT_PLAYER);
		getPlayer->ScalePlus(m_Scale);

		m_IncludeStage->DeleteEatting(m_Index);

		SAFE_RELEASE(getPlayer);
	}
}