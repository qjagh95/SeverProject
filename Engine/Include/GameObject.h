#pragma once
#include "RefCount.h"
JEONG_BEGIN

class Layer;
class Scene;
class Component_Base;
class Transform_Com;
class JEONG_DLL GameObject : public RefCount
{
public:
	bool Init();
	int Input(float DeltaTime);
	int Update(float DeltaTime);
	int LateUpdate(float DeltaTime);
	void Collision(float DeltaTime);
	void CollisionLateUpdate(float DeltaTime);
	void Render(float DeltaTime);
	GameObject* Clone(); //Ŭ���� ������������ ������Ÿ������.
	void AfterClone();

	Scene* GetScene() const { return m_Scene; }
	Layer* GetLayer() const { return m_Layer; }
	string GetLayerName() const { return m_LayerName; }
	int GetLayerZOrder() const { return m_LayerZOrder; }

	void SetScene(Scene* scene);
	void SetLayer(Layer* layer);

	Transform_Com* GetTransform() const { return m_Transform; }
	void SetTransform(Transform_Com* transform);

	void SetRotation(const Vector3& vecRot);
	void SetRotationX(float RotX);
	void SetRotationY(float RotY);
	void SetRotationZ(float RotZ);
	static GameObject* CreateObject(const string& TagName, Layer* layer = NULLPTR, bool isStaticObject = false);

	const list<Component_Base*>* GetComponentList() const;
	bool CheckComponentType(COMPONENT_TYPE eType);

	/////////////////////////////////////������Ÿ���Լ�(Clone)/////////////////////////////////////
	static GameObject* CreateProtoType(const string& TagName, bool isCurrent = true);
	static GameObject* CreateClone(const string& TagName, const string& ProtoTypeTagName, Layer* layer = NULLPTR, bool isCurrent = true);
	static void DestroyProtoType(Scene* scene);
	static void DestroyProtoType(Scene* scene, const string& TagName);
	static void DestroyProtoType();
	///////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////AddComponent�Լ�//////////////////////////////////////////
	Component_Base* AddComponent(Component_Base* component);
	template<typename T>
	T* AddComponent(const string& TagName)
	{
		T* newComponent = new T();
		newComponent->SetTag(TagName);
		newComponent->m_Scene = m_Scene;
		newComponent->m_Layer = m_Layer;
		newComponent->m_Transform = m_Transform;
		newComponent->m_Object = this;

		if (newComponent->Init() == false)
		{
			SAFE_RELEASE(newComponent);
			return NULLPTR;
		}
		return (T*)AddComponent(newComponent);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////FindComponent�Լ�/////////////////////////////////////////
	const list<Component_Base*>* FindComponentFromTag(const string& TagName);
	const list<Component_Base*>* FindComponentFromType(COMPONENT_TYPE type);

	template<typename T>
	T* FindComponentFromTag(const string& TagName)
	{
		list<Component_Base*>::iterator StartIter = m_ComponentList.begin();
		list<Component_Base*>::iterator EndIter = m_ComponentList.end();

		for (; StartIter != EndIter; StartIter++)
		{
			if ((*StartIter)->GetTag() == TagName)
			{
				(*StartIter)->AddRefCount();
				return (T*)*StartIter;
			}
		}
		return NULLPTR;
	}

	template <typename T>
	T* FindComponentFromType(COMPONENT_TYPE eType)
	{
		list<Component_Base*>::iterator	StartIter = m_ComponentList.begin();
		list<Component_Base*>::iterator	EndIter = m_ComponentList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if ((*StartIter)->GetComType() == eType)
			{
				(*StartIter)->AddRefCount();
				return (T*)*StartIter;
			}
		}
		return NULLPTR;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	static GameObject* FindProtoType(Scene* scene, const string& TagName);
	static GameObject* FindObject(const string& TagName);

	void AddChild(GameObject* Child);
	void AddStaticObject() {}
	bool EmptyComponent() { return m_ComponentList.empty(); }

	void Save(BineryWrite& Writer);
	void Load(BineryRead& Reader);

private:
	list<Component_Base*> m_ComponentList;
	Transform_Com* m_Transform;

	Scene* m_Scene;
	Layer* m_Layer;
	string m_LayerName;
	int m_LayerZOrder;
	
	list<Component_Base*> m_FindComList;
	static unordered_map<Scene*, unordered_map<string, GameObject*>> m_ProtoTypeMap;
	GameObject* m_Parent;
	list<GameObject*> m_ChildList;

private:
	GameObject();
	GameObject(const GameObject& copyObject);
	~GameObject();
};

JEONG_END

