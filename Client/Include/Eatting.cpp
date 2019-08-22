#include "ClientHeader.h"
#include "Eatting.h"
#include <Component/ColliderRect_Com.h>

Eatting::Eatting()
{
	m_isUpdate = true;
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

	JEONG::ColliderRect_Com* Rect = m_Object->AddComponent<JEONG::ColliderRect_Com>("123");
	Rect->SetInfo(0, 10);
	SAFE_RELEASE(Rect);

	float S = static_cast<float>(RandomRange(0, 10));
	m_Transform->SetWorldScale(S, S, 1.0f);
	m_Transform->SetWorldPos(100.f, 100.f, 1.0f);

	float R = static_cast<float>(RandomRange(0, 255));
	float G = static_cast<float>(RandomRange(0, 255));
	float B = static_cast<float>(RandomRange(0, 255));

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetMaterial(Vector4(R / 255.0f, G / 255.0f, B / 255.0f, 1.0f));
	MaterialComponent->SetDiffuseTexture(0, "Circle", TEXT("Circle.png"));
	SAFE_RELEASE(MaterialComponent);

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
