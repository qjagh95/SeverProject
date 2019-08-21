#include "stdafx.h"
#include "CollsionManager.h"
#include "GameObject.h"
#include "Device.h"
#include "KeyInput.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include "Component/Transform_Com.h"
#include "Component/ColliderPoint_Com.h"

JEONG_USING
SINGLETON_VAR_INIT(CollsionManager)

CollsionManager::CollsionManager()
{
}

CollsionManager::~CollsionManager()
{
	Safe_Delete_Map(m_GroupMap);
}

bool CollsionManager::Init()
{
	CreateGroup("Default", Vector3(0.0f, 0.0f, 0.0f), Vector3(5000.f, 5000.f, 0.f), 10, 10, 1, CGT_2D);
	CreateGroup("BackGround", Vector3(0.0f, 0.0f, 0.0f), Vector3(5000.f, 5000.f, 0.f), 10, 10, 1, CGT_2D);
	CreateGroup("UI", Vector3(0.0f, 0.0f, 0.0f), Vector3((float)Device::Get()->GetWinSize().Width, (float)Device::Get()->GetWinSize().Width, 0.0f), 4, 4, 1, CGT_2D);

	return true;
}

bool CollsionManager::CreateGroup(const string & KeyName, const Vector3 & Min, const Vector3 & Max, int SpaceCountX, int SpaceCountY, int SpaceCountZ, COLLSION_GROUP_TYPE eType)
{
	CollsionGroup* newGroup = FindGroup(KeyName);

	if (newGroup != NULLPTR)
		return false;   

	newGroup = new CollsionGroup();
	newGroup->Type = eType;											//2D or 3D
	newGroup->SpaceCountX = SpaceCountX;							//X�� ���� ũ��
	newGroup->SpaceCountY = SpaceCountY;							//Y�� ���� ũ��
	newGroup->SpaceCountZ = SpaceCountZ;							//Z�� ���� ũ��
	newGroup->SpaceCount = SpaceCountX * SpaceCountY * SpaceCountZ; //��ü ���� ����
	newGroup->Max = Max;											//���� ��ü�� ũ��
	newGroup->Min = Min;											//�ּҰ� ex(0 0 0 ~ 1280 720 0)
	newGroup->Lenth = Max - Min;									//������ ������
	newGroup->SpaceLenth = newGroup->Lenth / Vector3((float)SpaceCountX, (float)SpaceCountY, (float)SpaceCountZ);
	//���� �ϳ��� ũ��. ������ / ���Ұ���

	//��ü ���ҵ� ���� ������ŭ �����Ҵ�.
	newGroup->SectionList = new CollsionSection[newGroup->SpaceCount];

	m_GroupMap.insert(make_pair(KeyName, newGroup));

	return true;
}

void CollsionManager::ChangeGroupType(const string& KeyName, COLLSION_GROUP_TYPE eType)
{
	CollsionGroup* getGroup = FindGroup(KeyName);

	if (getGroup == NULLPTR)
		return;

	getGroup->Type = eType;
}

void CollsionManager::AddCollsion(GameObject * object)
{
	//�ö��̴�(�浹ü)������Ʈ�� ���ٸ�
	if (object->CheckComponentType(CT_COLLIDER) == false)
		return;

	Scene* CurScene = SceneManager::Get()->GetCurScene();
	Transform_Com* getTransform = CurScene->GetMainCameraTransform();
	Vector3 CameraPos = getTransform->GetWorldPos();

	SAFE_RELEASE(CurScene);

	//�ö��̴� ������Ʈ ����Ʈ�� �����´� (�浹ü�� ������ �� �� �ֱ⶧����) (��ĳ����)
	const list<Component_Base*>* getComList = object->FindComponentFromType(CT_COLLIDER);

	list<Component_Base*>::const_iterator StartIter = getComList->begin();
	list<Component_Base*>::const_iterator EndIter = getComList->end();

	for (; StartIter != EndIter; StartIter++)
	{
		//�ڽ�������Ʈ�� ����ȯ. ����ó���� ���������ִ� ������ �ε����� ������ �����Ѵ�.
		((Collider_Com*)*StartIter)->ClearSectionIndex();
		//���� �����ִ� �׷��� ã�´�.
		CollsionGroup* getGroup = FindGroup(((Collider_Com*)*StartIter)->GetCollisionGroupName());

		if (getGroup == NULLPTR)
		{
			TrueAssert(true);
			return;
		}

		//���߿� �ʿ��ϸ� ī�޶� �浹���� ��ü�� �ٿ��༭ �浹������ ī�޶��� ��ġ�� 
		//�������� ���� �� �ֵ��� �Ѵ�.
		//if (getGroup->Type == CGT_2D)
		//{
		//	Vector3 cPos = Vector3((float)Device::Get()->GetWinSize().Width, (float)Device::Get()->GetWinSize().Height, 0.0f);
		//	CameraPos += cPos * 0.5f;
		//}

		//if (((Collider_Com*)*StartIter)->GetCollisionGroupName() == "UI")
		//	CameraPos = Vector3::Zero;

		//���� Pos�� ��Ÿ����.
		Vector3	SectionMin = ((Collider_Com*)*StartIter)->GetSectionMin();
		Vector3	SectionMax = ((Collider_Com*)*StartIter)->GetSectionMax();

		int	xMin = 0;
		int xMax = 0;
		int yMin = 0;
		int yMax = 0;
		int zMin = 0;
		int zMax = 0;

		//���� ��ġ�� ���� �����ε����� ���Ѵ�.
		xMin = (int)(SectionMin.x - getGroup->Min.x) / (int)getGroup->SpaceLenth.x;
		xMax = (int)(SectionMax.x - getGroup->Min.x) / (int)getGroup->SpaceLenth.x;
		yMin = (int)(SectionMin.y - getGroup->Min.y) / (int)getGroup->SpaceLenth.y;
		yMax = (int)(SectionMax.y - getGroup->Min.y) / (int)getGroup->SpaceLenth.y;

		//Z�� ������ �ϳ��� �־����. (���ε���� ����)
		if (getGroup->SpaceCountZ > 1)
		{
			zMin = (int)(SectionMin.z - getGroup->Min.z) / (int)getGroup->SpaceLenth.z;
			zMax = (int)(SectionMax.z - getGroup->Min.z) / (int)getGroup->SpaceLenth.z;
		}

		vector<int> Index;
		for (int z = zMin; z <= zMax; z++)
		{
			//����ó��
			if (z < 0 || z >= getGroup->SpaceCountZ)
				continue;

			for (int y = yMin; y <= yMax; y++)
			{
				//����ó��
				if (y < 0 || y >= getGroup->SpaceCountY)
					continue;

				for (int x = xMin; x <= xMax; x++)
				{
					//����ó��
					if (x < 0 || x >= getGroup->SpaceCountX)
						continue;

					//�ε��� ����
					int	idx = z * (getGroup->SpaceCountX * getGroup->SpaceCountY) + y * getGroup->SpaceCountX + x;
					//���� ���� ������ �ε����� �־��ش�.
					((Collider_Com*)*StartIter)->AddCollisionSection(idx);
					Index.push_back(idx);

					CollsionSection* getSection = &getGroup->SectionList[idx];
					
					//�迭 ���Ҵ����(����)
					if (getSection->Capacity == getSection->Size)
					{
						getSection->Capacity *= 2;

						Collider_Com** Temp = new Collider_Com*[getSection->Capacity];

						memcpy(Temp, getSection->ColliderList, sizeof(Collider_Com*) * getSection->Size);

						SAFE_DELETE_ARRARY(getSection->ColliderList);

						getSection->ColliderList = Temp;
					}
					getSection->ColliderList[getSection->Size] = (Collider_Com*)*StartIter;
					getSection->Size++;
				}//for(x)
			}//for(y)	
		}//for(z)
	}//for(ColliderList)
}

void CollsionManager::Collsion(float DeltaTime)
{
	//���콺�� ������Ʈ�� �浹ó��
	GameObject* MouseObject = KeyInput::Get()->GetMouseObject();
	ColliderPoint_Com* MouseWindowPoint = MouseObject->FindComponentFromTag<ColliderPoint_Com>("MouseWindow");

	MouseWindowPoint->ClearSectionIndex();

	CollsionGroup* getGroup = FindGroup("UI");
	Vector3 mPoint = MouseWindowPoint->GetInfo();

	int MouseSectionIndexX = (int)(mPoint.x / getGroup->SpaceLenth.x);
	int MouseSectionIndexY = (int)(mPoint.y / getGroup->SpaceLenth.y);

	bool isUiColl = false;

	//�ε����� ��ü���Ұ��� �ȿ� ������
	if (MouseSectionIndexX >= 0 && MouseSectionIndexX < getGroup->SpaceCountX && MouseSectionIndexY >= 0 && MouseSectionIndexY < getGroup->SpaceCountY)
	{
		int PointIndex = MouseSectionIndexY * getGroup->SpaceCountX + MouseSectionIndexX;
		CollsionSection* getSection = &getGroup->SectionList[PointIndex];
		
		MouseWindowPoint->AddCollisionSection(PointIndex);

		if (getSection->Size > 0)
		{
			//sort(pSection->pColliderArray, &pSection->pColliderArray[pSection->iSize - 1], CCollisionManager::SortZOrder);

			for (int i = 0; i < getSection->Size; i++)
			{
				Collider_Com* CollSrc = getSection->ColliderList[i];
				Collider_Com* CollDest = MouseWindowPoint;

				bool Pair = false;
				for (size_t a = 0; a < CollSrc->GetContinueTypeNameSize(); a++)
				{
					if (CollDest->GetMyTypeName() == CollSrc->GetContinueTypeName(a))
					{
						Pair = true;
						break;
					}
				}

				if (CollDest->Collsion(CollSrc, DeltaTime) == true && Pair == false)
				{
					//ó���浹�ɰ��
					if (CollSrc->CheckPrevCollision(CollDest) == false)
					{
						CollSrc->AddPrevCollision(CollDest);
						CollDest->AddPrevCollision(CollSrc);

						CollSrc->OnCollsionFirst(CollDest, DeltaTime);
						CollDest->OnCollsionFirst(CollSrc, DeltaTime);

						isUiColl = true;
					}
					//�����浹ü�� �������(�浹���ΰ��)
					else
					{
						CollSrc->OnCollsionDoing(CollDest, DeltaTime);
						CollDest->OnCollsionDoing(CollSrc, DeltaTime);

						isUiColl = true;
					}
				}
				else
				{
					if (CollSrc->CheckPrevCollision(CollDest) == true)
					{
						CollSrc->OnCollsionEnd(CollDest, DeltaTime);
						CollDest->OnCollsionEnd(CollSrc, DeltaTime);

						CollSrc->ErasePrevCollision(CollDest);
						CollDest->ErasePrevCollision(CollSrc);
					}
				}
			}
		}
		//�߰�.
		if (isUiColl == true)
		{
			for (int i = 0; i < getSection->Size; i++)
			{
				ColliderPoint_Com* MouseWorldPoint = MouseObject->FindComponentFromTag<ColliderPoint_Com>("MouseWorld");
				Collider_Com* CollSrc = getSection->ColliderList[i];
				Collider_Com* CollDest = MouseWorldPoint;

				CollSrc->OnCollsionEnd(CollDest, DeltaTime);

				CollSrc->ErasePrevCollision(CollDest);
				CollDest->ErasePrevCollision(CollSrc);

				SAFE_RELEASE(MouseWorldPoint);
			}
		}
	}

	SAFE_RELEASE(MouseWindowPoint);

	if (isUiColl == false)
	{
		ColliderPoint_Com* MouseWorldPoint = MouseObject->FindComponentFromTag<ColliderPoint_Com>("MouseWorld");
		MouseWorldPoint->ClearSectionIndex();

		unordered_map<string, CollsionGroup*>::iterator StartIter1 = m_GroupMap.begin();
		unordered_map<string, CollsionGroup*>::iterator EndIter1 = m_GroupMap.end();

		bool isMouseColl = false;
		//��ü �׷� �� ��ŭ �ݺ�����.
		for (; StartIter1 != EndIter1; StartIter1++)
		{
			if (StartIter1->first == "UI")
				continue;

			int MouseWorldSpaceIndexX = (int)(MouseWorldPoint->GetInfo().x / StartIter1->second->SpaceLenth.x);
			int MouseWorldSpaceIndexY = (int)(MouseWorldPoint->GetInfo().y / StartIter1->second->SpaceLenth.y);

			if (MouseWorldSpaceIndexX >= 0 && MouseWorldSpaceIndexY >= 0 && MouseWorldSpaceIndexX < StartIter1->second->SpaceCountX && MouseWorldSpaceIndexY < StartIter1->second->SpaceCountY)
			{
				int MouseWorldIndex = MouseWorldSpaceIndexY * StartIter1->second->SpaceCountX + MouseWorldSpaceIndexX;
				MouseWorldPoint->AddCollisionSection(MouseWorldIndex);

				//���������� ������ �����´�.
				CollsionSection* getSection = &StartIter1->second->SectionList[MouseWorldIndex];

				for (int j = 0; j < getSection->Size; j++)
				{
					//�� ���� �ȿ��ִ� �浹ü ����Ʈ�� ������.
					Collider_Com* CollSrc = getSection->ColliderList[j];
					Collider_Com* CollDest = MouseWorldPoint;

					bool Pair = false;
					for (size_t a = 0; a < CollSrc->GetContinueTypeNameSize(); a++)
					{
						if (CollDest->GetMyTypeName() == CollSrc->GetContinueTypeName(a))
						{
							Pair = true;
							break;
						}
					}

					if (isMouseColl == true)
					{
						if (CollSrc->CheckPrevCollision(CollDest) == true && Pair == false)
						{
							CollSrc->OnCollsionEnd(CollDest, DeltaTime);
							CollDest->OnCollsionEnd(CollSrc, DeltaTime);

							CollSrc->ErasePrevCollision(CollDest);
							CollDest->ErasePrevCollision(CollSrc);
						}
						continue;
					}

					if (CollDest->Collsion(CollSrc, DeltaTime) == true)
					{
						//ó���浹�ɰ��
						if (CollSrc->CheckPrevCollision(CollDest) == false)
						{
							CollSrc->AddPrevCollision(CollDest);
							CollDest->AddPrevCollision(CollSrc);

							CollSrc->OnCollsionFirst(CollDest, DeltaTime);
							CollDest->OnCollsionFirst(CollSrc, DeltaTime);
							 
							isMouseColl = true;
						}
						else
						{
							CollSrc->OnCollsionDoing(CollDest, DeltaTime);
							CollDest->OnCollsionDoing(CollSrc, DeltaTime);

							isMouseColl = true;
						}
					}
					else
					{
						if (CollSrc->CheckPrevCollision(CollDest) == true)
						{
							CollSrc->OnCollsionEnd(CollDest, DeltaTime);
							CollDest->OnCollsionEnd(CollSrc, DeltaTime);

							CollSrc->ErasePrevCollision(CollDest);
							CollDest->ErasePrevCollision(CollSrc);
						}
					}//else
				}//for(j)
			}
		}
		//sort(iterG->second->pSection[i]->pColliderArray, &iterG->second->pSection[i]->pColliderArray[iterG->second->pSection[i]->iSize - 1],CCollisionManager::SortZ);
		SAFE_RELEASE(MouseWorldPoint);
	}

	unordered_map<string, CollsionGroup*>::iterator StartIter = m_GroupMap.begin();
	unordered_map<string, CollsionGroup*>::iterator EndIter = m_GroupMap.end();

	//��ü �׷� �� ��ŭ �ݺ�����.
	for (; StartIter != EndIter; StartIter++)
	{
		//��ü ���ҵ� ������ŭ �ݺ�����.
		for (int i = 0; i < StartIter->second->SpaceCount; i++)
		{
			CollsionSection* getSection = &StartIter->second->SectionList[i];

			//�浹ü���� 1�����ϸ� ���ʿ䰡����.
			if (getSection->Size < 2)
			{
				//������ Ʋ������ ������Ѵ�.
				//4���� ��ü�� �浹���� �÷��̾ �̵� �� �ٸ� �����ε����� �Űܰ��µ�
				//4���� ��ü�� �浹�Ǽ� ������� �÷��̾ü�� ������ Ʋ�������� �����ö��̴��� �˻��ؼ�
				//�ִٸ� �浹��ܶ��̴� End�Լ��� �ѹ� ȣ���ؾ��Ѵ�.

				//TODO
				for (int j = 0; j < getSection->Size; j++)
					getSection->ColliderList[j]->CheckPrevCollisionInSection(DeltaTime);

				getSection->Size = 0;
				continue;
			}

			//TODO
			for (int j = 0; j < getSection->Size; j++)
				getSection->ColliderList[j]->CheckPrevCollisionInSection(DeltaTime);

			//�� ���� �� �浹ü ����ŭ �ݺ��Ѵ�.
			//�������Ĳ�.
			for (int j = 0; j < getSection->Size - 1; j++)
			{
				for (int k = j + 1; k < getSection->Size; k++)
				{
					GameObject* Src = getSection->ColliderList[j]->GetGameObject();
					GameObject* Dest = getSection->ColliderList[k]->GetGameObject();

					//���� ���� ���� �浹ü�� �浹�Ѵٸ�..? �����ȵ�
					if (Src == Dest)
						continue;

					Collider_Com* CollSrc = getSection->ColliderList[j];
					Collider_Com* CollDest = getSection->ColliderList[k];

					bool Pair = false;
					for (size_t a = 0; a < CollDest->GetContinueTypeNameSize(); a++)
					{
						if (CollSrc->GetMyTypeName() == CollDest->GetContinueTypeName(a))
						{
							Pair = true;
							break;
						}
					}

					if (CollSrc->Collsion(CollDest, DeltaTime) == true && Pair == false)
					{
						//ó���浹�ɰ��
						if (CollSrc->CheckPrevCollision(CollDest) == false)
						{
							CollSrc->AddPrevCollision(CollDest);
							CollDest->AddPrevCollision(CollSrc);

							CollSrc->OnCollsionFirst(CollDest, DeltaTime);
							CollDest->OnCollsionFirst(CollSrc, DeltaTime);
						}
						//�����浹ü�� �������(�浹���ΰ��)
						else
						{
							CollSrc->OnCollsionDoing(CollDest, DeltaTime);
							CollDest->OnCollsionDoing(CollSrc, DeltaTime);
						}
					}
					//�浹 ���ϰ��ִµ� ���� �浹ü�� �������
					else
					{
						if (CollSrc->CheckPrevCollision(CollDest) == true)
						{
							CollSrc->OnCollsionEnd(CollDest, DeltaTime);
							CollDest->OnCollsionEnd(CollSrc, DeltaTime);

							CollSrc->ErasePrevCollision(CollDest);
							CollDest->ErasePrevCollision(CollSrc);
						}
					}
				}//for(k)
			}//for(j)
			getSection->Size = 0;
		}//for(SectionCount)
	}//for(GroupCount)
}

CollsionManager::CollsionGroup * CollsionManager::FindGroup(const string & KeyName)
{
	unordered_map<string, CollsionGroup*>::iterator FindIter = m_GroupMap.find(KeyName);

	if (FindIter == m_GroupMap.end())
		return NULLPTR;

	return FindIter->second;
}
