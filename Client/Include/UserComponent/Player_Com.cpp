#include "ClientHeader.h"
#include "Player_Com.h"
#include "GameObject.h"
#include "KeyInput.h"

#include "Component/ColliderRect_Com.h"
#include "Component/ColliderCircle_Com.h"
#include "Component/ColliderOBB2D_Com.h"

Player_Com::Player_Com()
{
}

Player_Com::Player_Com(const Player_Com & userCom)
	: BasicInfo(userCom)
{
}

Player_Com::~Player_Com()
{
}

bool Player_Com::Init()
{				   
	KeyInput::Get()->AddKey("S1", VK_F1);
	KeyInput::Get()->AddKey("S2", VK_F2);
	KeyInput::Get()->AddKey("S3", VK_F3);
	KeyInput::Get()->AddKey("S4", VK_F4);

	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("PlayerRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

	ColliderCircle_Com* Circle = m_Object->AddComponent<ColliderCircle_Com>("123");
	Circle->SetInfo(50.0f);
	Circle->SetCollsionCallback(CCT_FIRST, this, &Player_Com::EattingFunc);
	SAFE_RELEASE(Circle);

	m_Transform->SetWorldScale(100.0f, 100.0f, 1.0f);
	m_Transform->SetWorldPos(500.0f, 500.0f, 1.0f);

	float R = static_cast<float>(RandomRange(0, 255));
	float G = static_cast<float>(RandomRange(0, 255));
	float B = static_cast<float>(RandomRange(0, 255));

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetMaterial(Vector4(R / 255.0f, G / 255.0f, B / 255.0f, 1.0f));
	MaterialComponent->SetDiffuseTexture(0, "Circle", TEXT("Circle.png"));
	SAFE_RELEASE(MaterialComponent);

	return true;
}

int Player_Com::Input(float DeltaTime)
{
	if (KeyInput::Get()->KeyPress("MoveUp"))
		m_Transform->Move(AXIS_Y, 500.0f, DeltaTime);
	else if (KeyInput::Get()->KeyPress("MoveDown"))
		m_Transform->Move(AXIS_Y, -500.0f, DeltaTime);

	if (KeyInput::Get()->KeyPress("MoveLeft"))
		m_Transform->Move(AXIS_X, -500.0f, DeltaTime);
	else if (KeyInput::Get()->KeyPress("MoveRight"))
		m_Transform->Move(AXIS_X, 500.0f, DeltaTime);

	return 0;
}

int Player_Com::Update(float DeltaTime)
{

	return 0;
}

int Player_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Player_Com::Collision(float DeltaTime)
{
}

void Player_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Player_Com::Render(float DeltaTime)
{
}

Player_Com * Player_Com::Clone()
{
	return new Player_Com(*this);
}

void Player_Com::EattingFunc(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
}
