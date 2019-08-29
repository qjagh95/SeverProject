#include "stdafx.h"
#include "Stage_Com.h"
#include "Eatting_Com.h"
#include "Player_Com.h"

#include "Core.h"

JEONG_USING
__int64 Stage_Com::m_Count = 0;

Stage_Com::Stage_Com()
{

}

Stage_Com::Stage_Com(const Stage_Com & CopyData)
	:UserComponent_Base(CopyData)
{
	Safe_Release_VecList(m_vecAllEatting);
}

Stage_Com::~Stage_Com()
{
}

bool Stage_Com::Init()
{
	m_WinSize.x = Core::Get()->GetWinSize().Width;
	m_WinSize.y = Core::Get()->GetWinSize().Height;

	return true;
}

int Stage_Com::Input(float DeltaTime)
{
	m_UpdateEatting.clear();

	Vector3 CameraPos = m_Scene->GetMainCameraTransform()->GetWorldPos();
	Vector3 PlusPos;
	PlusPos.x = CameraPos.x + m_WinSize.x;
	PlusPos.y = CameraPos.y + m_WinSize.y;

	//가시판단
	for (auto CurObject : m_vecAllEatting)
	{
		Vector3 getPos = CurObject->GetTransform()->GetWorldPos();

		if ((getPos.x >= CameraPos.x && getPos.x <= PlusPos.x) && (getPos.y >= CameraPos.y && getPos.y <= PlusPos.y))
			m_UpdateEatting.push_back(CurObject);

		else
			continue;
	}

	for (size_t i = 0; i < m_UpdateEatting.size(); i++)
	{
		if (m_UpdateEatting[i]->GetIsDelete() == true)
		{
			SAFE_RELEASE(m_UpdateEatting[i]);
			m_UpdateEatting.erase(m_UpdateEatting.begin() + i);
		}
		else
			m_UpdateEatting[i]->Input(DeltaTime);
	}

	return 0;
}

int Stage_Com::Update(float DeltaTime)
{
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

void Stage_Com::CreateEatting(const Vector3& Pos, const Vector3& RGB, float Scale)
{
	char Buffer[12] = {};
	itoa(m_Count, Buffer, 10);

	string Convert = Buffer;                  

	GameObject* newObject = GameObject::CreateObject(Convert, NULLPTR);
	newObject->SetScene(m_Scene);
	newObject->SetLayer(m_Layer);

	Eatting_Com* newEatting = newObject->AddComponent<Eatting_Com>("Eatting" + Convert);
	newEatting->GetTransform()->SetWorldPos(Pos);
	newEatting->SetRGB(RGB.x, RGB.y, RGB.z);
	newEatting->SetCollScale(Scale * 0.5f);
	newEatting->SetStage(this);

	m_vecAllEatting.push_back(newObject);
	m_AllEattingMap.insert(make_pair(m_Count, newObject));

	m_Count++;
}

GameObject * Stage_Com::FindEatting(__int64 Key)
{
	auto FindIter = m_AllEattingMap.find(Key);

	if (FindIter == m_AllEattingMap.end())
		return NULLPTR;

	return FindIter->second;
}

void Stage_Com::DeleteEatting(__int64 Key)
{
	GameObject* getObject = FindEatting(Key);

	if (getObject == NULLPTR)
		return;

	m_AllEattingMap.erase(Key);
	m_vecAllEatting.erase(m_vecAllEatting.begin() + Key);
	
	getObject->SetIsDelete(true);
}
