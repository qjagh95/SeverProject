#pragma once
#include <WinSock2.h>
#include <mswsock.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <crtdbg.h>
#include <conio.h>
#include <assert.h>
#include <array>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <atlstr.h>
#include <process.h>

#include "MathHeader.h"
#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>
#include <Matrix.h>

//DX11부터는 포함디렉터리 라이브러리 디렉터리 추가없이 바로 헤더추가가 가능하다.
//이전에는 프로젝트 템플릿(VS기능)을 이용하여 프로젝트를 먼저 만들어놓고 추가해서 사용했다.
#include <d3d11.h>
#include <d3dcompiler.h> //shader를 컴파일하는 기능을 제공한다.
#include <d2d1.h>
#include <dwrite.h>
#include <dxgidebug.h>

#include <Audio.h>

#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
//DWrite
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma warning(disable:4099)

#include "Macro.h"
#include "Flag.h"
#include "Type.h"
#include "Header.h"

#define JEONG_BEGIN namespace JEONG {
#define JEONG_END }
#define JEONG_USING using namespace JEONG;
#define TrueAssert(Var) assert(!(Var))

//#ifdef _DEBUG
//#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)
//#endif // DEBUG

#include "Device.h"
#include "PathManager.h"
#include "TimeManager.h"
#include "CollsionManager.h"
#include "KeyInput.h"
#include "GameObject.h"
#include "BineryWriter.h"
#include "BineryReader.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"

#include "Component/Material_Com.h"
#include "Component/Renderer_Com.h"
#include "Component/Transform_Com.h"
#include "Component/Camera_Com.h"
#include "Component/Collider_Com.h"

#include "UserComponent/UserComponent_Base.h"

#include "Resource/ResourceManager.h"
#include "Resource/Mesh.h"

#include "Render/RenderManager.h"
#include "Render/ShaderManager.h"
#include "Render/Shader.h"
#include "Render/DepthStancilState.h"
#include "Render/BlendState.h"
