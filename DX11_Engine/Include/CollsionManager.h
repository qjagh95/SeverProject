#pragma once
#include "Component\Collider_Com.h"

JEONG_BEGIN

enum JEONG_DLL COLLSION_GROUP_TYPE
{
	CGT_2D,
	CGT_3D,
};

class GameObject;
class JEONG_DLL CollsionManager
{
private:
	//�ϳ��� ���� �� ��ü�� �ǹ��ϴ� ����. Collider�� �����͸� �޾ƾ��ϱ⶧���� 2�������� (vector��� ����)
	struct CollsionSection
	{
		Collider_Com** ColliderList;
		int Size;					 //�浹ü ī��Ʈ
		int Capacity;

		CollsionSection() : Size(0), Capacity(5) { ColliderList = new Collider_Com*[Capacity]; }
		~CollsionSection() { SAFE_DELETE_ARRARY(ColliderList); }
	};
	//��ü�����̴�.
	struct CollsionGroup
	{
		COLLSION_GROUP_TYPE Type;
		CollsionSection* SectionList;
		int SpaceCountX;
		int SpaceCountY;
		int SpaceCountZ;
		int SpaceCount;
		Vector3 Min;		//��ü������ �ּҰ�
		Vector3 Max;	    //��ü������ �ִ밪
		Vector3 Lenth;		//Max - Min = ����
		Vector3 SpaceLenth; //���� �ϳ��� ũ��

		CollsionGroup() {}
		~CollsionGroup() {	SAFE_DELETE_ARRARY(SectionList); }
	};

public:
	bool Init();
	bool CreateGroup(const string& KeyName, const Vector3& Min, const Vector3& Max, int SpaceCountX, int SpaceCountY, int SpaceCountZ, COLLSION_GROUP_TYPE eType = CGT_3D);
	void ChangeGroupType(const string& KeyName, COLLSION_GROUP_TYPE eType);
	void AddCollsion(GameObject* object);
	void ClearCollsionGroup() { Safe_Delete_Map(m_GroupMap); }
	void Collsion(float DeltaTime);

private:
	//�׷��ϳ��ϳ��� �Ȱ��� ������ü �ϳ��� �����Ѵٰ� �����ϸ� ���ϴ�.
	unordered_map<string, CollsionGroup*> m_GroupMap;

private:
	CollsionGroup* FindGroup(const string& KeyName);

public:
	CLASS_IN_SINGLE(CollsionManager)
};

JEONG_END