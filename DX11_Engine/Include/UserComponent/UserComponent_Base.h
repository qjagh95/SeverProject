#pragma once
#include "../Component/Component_Base.h"

JEONG_BEGIN
//����������Ʈ�� �̳��� ��ӹ޴´�.
class JEONG_DLL UserComponent_Base : public Component_Base
{
public:
	 virtual bool Init() override;
	 virtual int Input(float DeltaTime) override;
	 virtual int Update(float DeltaTime) override;
	 virtual int LateUpdate(float DeltaTime) override;
	 virtual void Collision(float DeltaTime) override;
	 virtual void Render(float DeltaTime) override;
	 virtual UserComponent_Base* Clone() override;
	 virtual void AfterClone() override {}

protected:
	Header m_MessageType;

protected:
	UserComponent_Base();
	UserComponent_Base(const UserComponent_Base& userCom);
	virtual ~UserComponent_Base();
};

JEONG_END

