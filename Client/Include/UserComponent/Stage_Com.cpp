#include "ClientHeader.h"
#include "Stage_Com.h"
#include "Eatting_Com.h"
#include "Player_Com.h"

#include "Core.h"

Stage_Com::Stage_Com()
{
	m_MainPlayer = NULLPTR;
	m_TimeVar = 0.0f;
	m_Sec = 0.5f;
	m_isTimeCheck = false;
	m_isCheck = false;
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
	m_WinSize.x = static_cast<float>(Core::Get()->GetWinSize().Width);
	m_WinSize.y = static_cast<float>(Core::Get()->GetWinSize().Height);

	return true;
}

int Stage_Com::Input(float DeltaTime)
{
	if (m_MainPlayer == NULLPTR)
		return 0;
	
	if(m_isTimeCheck == false)
		m_TimeVar += DeltaTime;

	if (m_TimeVar >= m_Sec)
		m_isCheck = true;

	if (m_isCheck == false)
		return 0;

	Vector3 CameraPos = m_Scene->GetMainCameraTransform()->GetWorldPos();
	Vector3 PlusPos;
	PlusPos.x = CameraPos.x + m_WinSize.x;
	PlusPos.y = CameraPos.y + m_WinSize.y;

	bool isCheck = false;

	for (size_t i = 0; i < m_UpdateEatting.size(); i++)
	{
		if (m_UpdateEatting[i]->GetIsDelete() == true)
		{
			SAFE_RELEASE(m_UpdateEatting[i]);
			m_UpdateEatting.erase(m_UpdateEatting.begin() + i);
		}

		Vector3 EatPos = m_UpdateEatting[i]->GetTransform()->GetWorldPos();

		if ((EatPos.x >= CameraPos.x && EatPos.x <= PlusPos.x) && (EatPos.y >= CameraPos.y && EatPos.y <= PlusPos.y))
			m_UpdateEatting[i]->Input(DeltaTime);
		else
		{
			m_UpdateEatting[i]->SetIsActive(false);
			SAFE_RELEASE(m_UpdateEatting[i]);
			m_UpdateEatting.erase(m_UpdateEatting.begin() + i);
		}
	}

	return 0;
}

int Stage_Com::Update(float DeltaTime)
{
	if (m_MainPlayer == NULLPTR)
		return 0;
	if (m_isCheck == false)
		return 0;

	for (size_t i = 0; i < m_UpdateEatting.size(); i++)
	{
		if (m_UpdateEatting[i]->GetIsDelete() == true)
		{
			SAFE_RELEASE(m_UpdateEatting[i]);
			m_UpdateEatting.erase(m_UpdateEatting.begin() + i);
		}
		else
			m_UpdateEatting[i]->Update(DeltaTime);
	}

	return 0;
}

int Stage_Com::LateUpdate(float DeltaTime)
{
	if (m_isCheck == false)
		return 0;

	for (size_t i = 0; i < m_UpdateEatting.size(); i++)
	{
		if (m_UpdateEatting[i]->GetIsDelete() == true)
		{
			SAFE_RELEASE(m_UpdateEatting[i]);
			m_UpdateEatting.erase(m_UpdateEatting.begin() + i);
		}
		else
			m_UpdateEatting[i]->LateUpdate(DeltaTime);
	}

	return 0;
}

void Stage_Com::Collision(float DeltaTime)
{
}

void Stage_Com::Render(float DeltaTime)
{
	if (m_MainPlayer == NULLPTR)
		return;

	if (m_isCheck == false)
		return;

	for (size_t i = 0; i < m_UpdateEatting.size(); i++)
	{
		if (m_UpdateEatting[i]->GetIsDelete() == true)
		{
			SAFE_RELEASE(m_UpdateEatting[i]);
			m_UpdateEatting.erase(m_UpdateEatting.begin() + i);
		}
		else
			m_UpdateEatting[i]->Collision(DeltaTime);
	}

	for (size_t i = 0; i < m_UpdateEatting.size(); i++)
	{
		if (m_UpdateEatting[i]->GetIsDelete() == true)
		{
			SAFE_RELEASE(m_UpdateEatting[i]);
			m_UpdateEatting.erase(m_UpdateEatting.begin() + i);
		}
		else
			m_UpdateEatting[i]->Render(DeltaTime);
	}
}

Stage_Com * Stage_Com::Clone()
{
	return nullptr;
}

void Stage_Com::CreateEatting(const Vector3& Pos, const Vector4& RGB, size_t ID)
{
	bool isUpdateOverapped = false;
	for (size_t i = 0; i < m_UpdateEatting.size(); i++)
	{
		Eatting_Com* getCom = m_UpdateEatting[i]->FindComponentFromType<Eatting_Com>(CT_EATTING);

		if (getCom->GetID() == ID)
		{
			isUpdateOverapped = true;
			SAFE_RELEASE(getCom);
			break;
		}
		
		SAFE_RELEASE(getCom);
	}

	if (isUpdateOverapped == false)
	{
		GameObject* newObject = GameObject::CreateObject("Eatting", NULLPTR);
		newObject->SetScene(m_Scene);
		newObject->SetLayer(m_Layer);

		Eatting_Com* newEatting = newObject->AddComponent<Eatting_Com>("Eatting");
		newEatting->GetTransform()->SetWorldPos(Pos);
		newEatting->SetRGB(RGB.x, RGB.y, RGB.z);
		newEatting->SetStage(this);

		m_UpdateEatting.push_back(newObject);
	}
}