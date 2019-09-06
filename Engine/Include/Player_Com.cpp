#include "stdafx.h"
#include "Player_Com.h"
#include "Component/ColliderCircle_Com.h"
#include "MessageManager.h"

JEONG_USING
Player_Com::Player_Com()
{
	m_ComType = CT_PLAYER;
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

	m_CirCleColl= m_Object->AddComponent<ColliderCircle_Com>("PlayerCircle");

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "Circle", TEXT("Circle.png"));
	SAFE_RELEASE(MaterialComponent);

	return true;
}

int Player_Com::Input(float DeltaTime)
{
	return 0;
}

int Player_Com::Update(float DeltaTime)
{
	Move(DeltaTime);

	return 0;
}

int Player_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Player_Com::Collision(float DeltaTime)
{
}

void Player_Com::Render(float DeltaTime)
{
}

Player_Com * Player_Com::Clone()
{
	return new Player_Com(*this);
}

void Player_Com::ScalePlus(float Scale)
{
	m_Transform->AddScale(Scale);
	m_CirCleColl->AddInfo(Scale * 0.5f);
}

void Player_Com::Move(float DeltaTime)
{
	if (KeyInput::Get()->KeyDown("MoveUp"))
		MessageManager::Get()->Client_ClientDie();

	if (KeyInput::Get()->KeyPress("MoveUp"))
	{
		if (m_Transform->GetWorldPos().y < 50000.0f)
			m_Transform->Move(AXIS_Y, 100.0f, DeltaTime);
		
		if(m_Transform->GetWorldPos().y >= 50000.0f)
			m_Transform->SetWorldPos(0.0f, 50000.0f, 1.0f);
	}
	else if (KeyInput::Get()->KeyPress("MoveDown"))
	{
		if (m_Transform->GetWorldPos().y > 0.0f)
			m_Transform->Move(AXIS_Y, -100.0f, DeltaTime);

		if (m_Transform->GetWorldPos().y < 0.0f)
			m_Transform->SetWorldPos(m_Transform->GetWorldPos().x, 0.0f, 1.0f);
	}

	if (KeyInput::Get()->KeyPress("MoveLeft"))
	{
		if (m_Transform->GetWorldPos().x > 0.0f)
			m_Transform->Move(AXIS_X, -100.0f, DeltaTime);

		if (m_Transform->GetWorldPos().x < 0.0f)
			m_Transform->SetWorldPos(0.0f, m_Transform->GetWorldPos().y, 1.0f);
	}
	else if (KeyInput::Get()->KeyPress("MoveRight"))
	{
		if (m_Transform->GetWorldPos().x < 50000.0f)
			m_Transform->Move(AXIS_X, 100.0f, DeltaTime);

		if (m_Transform->GetWorldPos().x > 50000.0f)
			m_Transform->SetWorldPos(50000.0f, m_Transform->GetWorldPos().y, 1.0f);
	}
}
