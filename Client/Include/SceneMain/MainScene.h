#pragma once
#include "Scene/SceneComponent.h"
#include "UserComponent/Stage_Com.h"

JEONG_USING
class MainScene : public SceneComponent
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;  
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;

private:
	Stage_Com* m_MainStage;

public:
	MainScene();
	~MainScene();
};


