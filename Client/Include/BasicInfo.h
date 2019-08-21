#pragma once
#include <UserComponent/UserComponent_Base.h>

class BasicInfo : public UserComponent_Base
{
public:
	virtual bool Init() override;
	virtual int Input(float DeltaTime) override;
	virtual int Update(float DeltaTime) override;
	virtual int LateUpdate(float DeltaTime) override;
	virtual void Collision(float DeltaTime) override;
	virtual void CollisionLateUpdate(float DeltaTime) override;
	virtual void Render(float DeltaTime) override;
	virtual BasicInfo* Clone() override;
	void SendPos();
	void SendScale();
	void RecvPos();
	void RecvRGB();
	void RecvScale();

protected:
	float m_Scale;
	Vector3 m_RGB;

public:
	BasicInfo();
	~BasicInfo();
};

