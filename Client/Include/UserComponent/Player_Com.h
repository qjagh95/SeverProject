#pragma once
#include "BasicInfo.h"

#include <Component/ColliderCircle_Com.h>
JEONG_USING

class Player_Com : public BasicInfo
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Player_Com* Clone() override;

	void EattingFunc(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

protected:
	Player_Com();
	Player_Com(const Player_Com& userCom);
	~Player_Com();

public:
	friend class GameObject;
};

