#pragma once
#include "BasicInfo.h"

JEONG_BEGIN
class JEONG_DLL Player_Com : public BasicInfo
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Player_Com* Clone() override;
	
	void ScalePlus(float Scale);

private:
	void Move(float DeltaTime);

private:
	

protected:
	Player_Com();
	Player_Com(const Player_Com& userCom);
	~Player_Com();

public:
	friend class GameObject;
};

JEONG_END