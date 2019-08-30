#pragma once

#include "UserComponent/UserComponent_Base.h"
JEONG_BEGIN

class ColliderCircle_Com;
class ColliderRect_Com;
class JEONG_DLL BasicInfo : public UserComponent_Base
{
public:
	virtual bool Init() override;
	virtual int Input(float DeltaTime) override;
	virtual int Update(float DeltaTime) override;
	virtual int LateUpdate(float DeltaTime) override;
	virtual void Collision(float DeltaTime) override;
	virtual void Render(float DeltaTime) override;
	virtual BasicInfo* Clone() override;

	void SetRGB(float R, float G, float B);
	void SetScale(float Scale);
	void SetCollScale(float Scale);
	Vector4 GetRGB() const { return m_RGB; }

protected:
	float m_Scale;
	float m_CollScale;
	Vector4 m_RGB;

	ColliderCircle_Com* m_CirCleColl;
	ColliderRect_Com* m_RectColl;
	Material_Com* m_Material;

	float m_CollVar;

public:
	BasicInfo();
	virtual ~BasicInfo();
};

JEONG_END

