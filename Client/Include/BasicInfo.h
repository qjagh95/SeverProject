#pragma once
#include <UserComponent/UserComponent_Base.h>
#include <Component/ColliderCircle_Com.h>
#include <Component/ColliderRect_Com.h>

class BasicInfo : public UserComponent_Base
{
public:
	virtual bool Init() override;
	virtual int Input(float DeltaTime) override;
	virtual int Update(float DeltaTime) override;
	virtual int LateUpdate(float DeltaTime) override;
	virtual void Collision(float DeltaTime) override;
	virtual void Render(float DeltaTime) override;
	virtual BasicInfo* Clone() override;

	void SendPos();
	void SendScale();
	void RecvPos();
	void RecvRGB();
	void RecvScale();

	void SetRGB(float R, float G, float B);
	void SetScale(float Scale);

protected:
	float m_Scale;
	float m_CollScale;
	Vector3 m_RGB;

	ColliderCircle_Com* m_CirCleColl;
	ColliderRect_Com* m_RectColl;
	Material_Com* m_Material;

	float m_CollVar;
public:
	BasicInfo();
	virtual ~BasicInfo();
};

