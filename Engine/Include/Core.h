#pragma once
JEONG_BEGIN

class JEONG_DLL Core
{
public:
	bool Init(HINSTANCE hInst, unsigned int Width,	unsigned int Height, const TCHAR* TitleName, const TCHAR* ClassName, int iIconID, int iSmallIconID,	bool bWindowMode = true);
	bool Init(HINSTANCE hInst, HWND hWnd, unsigned int Width, unsigned int Height, bool bWindowMode = true);
	int Run();
	void Logic();

	void SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetGameMode(GAME_MODE eMode);

	HWND GetHwnd() const { return m_hWnd; }
	HINSTANCE GetHinstance() const { return m_hIstance; }
	WinSize GetWinSize() const { return m_WinSize; }
	bool GetIsLoop() const { return m_isLoop; }
	default_random_engine GetRandomEngine() const { return m_RandomEngine; }
	int RandomRange(int R1, int R2);

public:
	PROJECT_TYPE m_ProjectType;

private:
	int Input(float DeltaTime);
	int Update(float DeltaTime);
	int LateUpdate(float DeltaTime);
	int Collsion(float DeltaTime);
	void Render(float DeltaTime);

	void Register(const TCHAR* ClassName, int iIconID, int iSmallIconID);
	void CreateWnd(const TCHAR* TitleName, const TCHAR* ClassName);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static bool m_isLoop;
	HINSTANCE m_hIstance;
	WinSize m_WinSize;
	HWND m_hWnd;
	float ClearColor[4];
	GAME_MODE m_GameMode;
	static WPARAM m_wParam;
	SocketInfo* m_ClientSocket;
	random_device m_RandomDevice;
	default_random_engine m_RandomEngine;

public:
	CLASS_IN_SINGLE(Core)
};

JEONG_END
