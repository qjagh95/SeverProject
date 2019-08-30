#include "ClientHeader.h"
#include "Core.h"
#include "resource.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include "SceneMain/MainScene.h"
#include <ConnectSever.h>
#include <messagemanager.h>

JEONG_USING

int APIENTRY wWinMain(_In_ HINSTANCE hIstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	if (Core::Get()->Init(hIstance, 1280, 720, TEXT("JeongRealEngine"), TEXT("JeongRealEngine"), IDI_ICON1, IDI_ICON1) == false)
	{
		Core::Delete();
		return 0;
	}

	Core::Get()->m_ProjectType = MY_IS_CLIENT;
	ConnectSever::Get()->Init();

	Core::Get()->SetGameMode(GM_2D);
	SceneManager::Get()->AddSceneComponent<MainScene>("MainScene");
	//MessageManager::Get()->ClientInit();

	int Result = Core::Get()->Run();
	Core::Delete();

	return Result;
}