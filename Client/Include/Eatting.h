#pragma once
#include "BasicInfo.h"
#include "Component/Collider_Com.h"

class Eatting : public BasicInfo
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Eatting* Clone() override;

public:
	Eatting();
	~Eatting();
};

