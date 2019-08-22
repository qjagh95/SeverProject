#include "ClientHeader.h"
#include "Stage_Com.h"
#include "Eatting.h"
#include "UserComponent/Player_Com.h"

#include <Core.h>

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

	return true;
}

int Stage_Com::Input(float DeltaTime)
{
	m_UpdateEatting.clear();

	Vector3 CameraPos = m_Scene->GetMainCameraTransform()->GetWorldPos();
	Vector3 PlusPos;
	PlusPos.x = CameraPos.x + m_WinSize.x;
	PlusPos.y = CameraPos.y + m_WinSize.y;

	//�����Ǵ�
	for (auto CurObject : m_vecAllEatting)
	{
		Vector3 getPos = CurObject->GetTransform()->GetWorldPos();

		if (CameraPos.x <= PlusPos.x && CameraPos.y <= PlusPos.y)
			m_UpdateEatting.push_back(CurObject);

		else
			continue;
	}

	for (auto CurObject : m_UpdateEatting)
		CurObject->Input(DeltaTime);

	return 0;
}

int Stage_Com::Update(float DeltaTime)
{
	for (auto CurObject : m_UpdateEatting)
		CurObject->Update(DeltaTime);

	return 0;
}

int Stage_Com::LateUpdate(float DeltaTime)
{
	for (auto CurObject : m_UpdateEatting)
		CurObject->LateUpdate(DeltaTime);

	return 0;
}

void Stage_Com::Collision(float DeltaTime)
{
}

void Stage_Com::Render(float DeltaTime)
{
	for (auto CurObject : m_UpdateEatting)
		CurObject->Collision(DeltaTime);

	for (auto CurObject : m_UpdateEatting)
		CurObject->Render(DeltaTime);
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

	Eatting* newEatting = newObject->AddComponent<Eatting>("Eatting" + Convert);
	newEatting->GetTransform()->SetWorldPos(Pos);
	newEatting->SetRGB(RGB.x, RGB.y, RGB.z);
	newEatting->SetCollScale(Scale);

	m_vecAllEatting.push_back(newObject);
	m_AllEattingMap.insert(make_pair(m_Count, newObject));

	m_Count++;
}
