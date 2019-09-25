#include "ClientHeader.h"
#include "Eatting_Com.h"
#include "Stage_Com.h"
#include "Player_Com.h"

Eatting_Com::Eatting_Com()
{
	m_isUpdate = true;
	m_CurStage = NULLPTR;
	m_ID = -1;
	m_ComType = CT_EATTING;
}

Eatting_Com::~Eatting_Com()
{
}

bool Eatting_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("PlayerRender");
	RenderComponent->SetMesh("TextureRect");
	SAFE_RELEASE(RenderComponent);

	BasicInfo::Init();

	m_Transform->SetWorldScale(5.0f);
	m_RectColl = m_Object->AddComponent<ColliderRect_Com>("EattingRect");
	m_RectColl->SetCollsionCallback(CCT_FIRST, this, &Eatting_Com::PlayerColl);
	m_RectColl->SetInfo(Vector3::Zero, Vector3(5.0f, 5.0f, 1.0f));

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

		//m_IncludeStage->DeleteEatting(m_Index);

		SAFE_RELEASE(getPlayer);
	}
}