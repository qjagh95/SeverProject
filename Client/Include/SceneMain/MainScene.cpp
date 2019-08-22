#include "ClientHeader.h"
#include "MainScene.h"
#include "GameObject.h"

#include "Scene/Scene.h"
#include "scene/Layer.h"

#include "Component/Component_Base.h"
#include "Component/ColliderPixel_Com.h"
#include "Component/BackColor_Com.h"
#include "Eatting.h"
#include "Stage_Com.h"

#include "../UserComponent/Player_Com.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

bool MainScene::Init()
{
	Camera_Com* mainCamera = m_Scene->GetMainCamera();
	mainCamera->SetCameraType(CT_ORTHO);
	mainCamera->SetNear(0.0f);

	KeyInput::Get()->SetShowCursor(false);

	Layer* BackLayer = m_Scene->FindLayer("BackGround");
	Layer* Default = m_Scene->FindLayer("Default");
	Layer* UILayer = m_Scene->FindLayer("UI");

	GameObject* BackObject = GameObject::CreateObject("BackObject", BackLayer);
	BackColor_Com* BackCom = BackObject->AddComponent<BackColor_Com>("BackColor");
	BackCom->SetBackColor(Vector4(100.0f, 50.0f, 255.0f, 255.0f));

	SAFE_RELEASE(BackCom);
	SAFE_RELEASE(BackObject);

	GameObject* PlayerObject = GameObject::CreateObject("Player", Default);
	Player_Com* player_Com = PlayerObject->AddComponent<Player_Com>("Player_Com");
	SAFE_RELEASE(player_Com);


	//GameObject* Eatting123 = GameObject::CreateObject("Eatting123123", Default);
	//Eatting* Eatting123123 = Eatting123->AddComponent<Eatting>("Player_Com");
	//Eatting123123->GetTransform()->SetWorldPos(Vector3(1000.0f, 500.0f, 1.0f));

	//SAFE_RELEASE(Eatting123);
	//SAFE_RELEASE(Eatting123123);
	
	GameObject* StageObj = GameObject::CreateObject("StageObj", Default);
	Stage_Com* MainStage = StageObj->AddComponent<Stage_Com>("Player_Com");
	MainStage->CreateEatting(Vector3(1000.0f, 500.0f ,1.0f), Vector3(255.0f, 255.0f, 255.0f), 10.0f);

	mainCamera->SetTarget(PlayerObject);
	SAFE_RELEASE(mainCamera);
	
	SAFE_RELEASE(Default);
	SAFE_RELEASE(UILayer);
	SAFE_RELEASE(BackLayer);

	return true;
}

int MainScene::Input(float DeltaTime)
{
	return 0;
}

int MainScene::Update(float DeltaTime)
{
	return 0;
}

int MainScene::LateUpdate(float DeltaTime)
{
	return 0;
}

void MainScene::Collision(float DeltaTime)
{
}

void MainScene::Render(float DeltaTime)
{
}