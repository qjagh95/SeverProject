#include "stdafx.h"
#include "Layer.h"
#include "Scene.h"

#include "../GameObject.h"

JEONG_USING

Layer::Layer()
	:m_ZOrder(-1)
{
}

Layer::~Layer()
{
	Safe_Release_VecList(m_ObjectList);
}

bool Layer::Init()
{
	return true;
}

int Layer::Input(float DeltaTime)
{
	list<GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->Input(DeltaTime);
		StartIter++;
	}

	return 0;
}

int Layer::Update(float DeltaTime)
{
	list<GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->Update(DeltaTime);
		StartIter++;
	}
	return 0;
}

int Layer::LateUpdate(float DeltaTime)
{
	list<GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->LateUpdate(DeltaTime);
		StartIter++;
	}
	return 0;
}

void Layer::Collision(float DeltaTime)
{
	list<GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->Collision(DeltaTime);
		StartIter++;
	}
}

void Layer::Render(float DeltaTime)
{
	list<GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}
		//���� Render�� RenderTarget���ٰ� �ȼ������� �����ش�.
		RenderManager::Get()->AddRenderObject((*StartIter));
		StartIter++;
	}
}

void Layer::SetZOrder(int zorder)
{
	m_ZOrder = zorder;
	m_Scene->SortLayer();
}

void Layer::AddObject(GameObject * object)
{
	object->SetScene(m_Scene);
	object->SetLayer(this);
	object->AddRefCount();

	m_ObjectList.push_back(object);
}

GameObject * Layer::FindObject(const string & TagName)
{
	list<GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ( ((*StartIter)->GetTag() == TagName))
		{
			(*StartIter)->AddRefCount();
			return *StartIter;
		}
	}
	return NULLPTR;
}
