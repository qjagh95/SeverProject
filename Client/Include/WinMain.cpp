#include "ClientHeader.h"
#include "Core.h"
#include "resource.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include "SceneMain/MainScene.h"

JEONG_USING

int APIENTRY wWinMain(_In_ HINSTANCE hIstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	if (Core::Get()->Init(hIstance, 1280, 720, TEXT("JeongRealEngine"), TEXT("JeongRealEngine"), IDI_ICON1, IDI_ICON1) == false)
	{
		Core::Delete();
		return 0;
	}

	////소켓연결준비
	//SOCKET ConnetSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULLPTR, 0, WSA_FLAG_OVERLAPPED);

	//WSADATA Temp;
	//WSAStartup(MAKEWORD(2, 2), &Temp);

	////서버주소
	//string IPAddress = "192.168.0.2";
	//sockaddr_in Addr = {};
	//Addr.sin_family = AF_INET;
	//Addr.sin_port = PORT;
	//inet_pton(AF_INET, IPAddress.c_str(), &Addr.sin_addr);

	//connect(ConnetSocket, reinterpret_cast<sockaddr*>(&Addr), sizeof(Addr));

	Core::Get()->SetGameMode(GM_2D);

	SceneManager::Get()->AddSceneComponent<MainScene>("MainScene");

	int Result = Core::Get()->Run();
	Core::Delete();

	return Result;
}