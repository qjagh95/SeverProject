#pragma once
#include "BasicInfo.h"
JEONG_USING

class Eatting : public BasicInfo
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Eatting* Clone() override;

	bool GetisUpdate() const { return m_isUpdate; }
	void SetRGB(float R, float G, float B);
	void SetScale(float Scale);

private:
	bool m_isUpdate;

public:
	Eatting();
	~Eatting();
};

