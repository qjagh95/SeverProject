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

//DX11���ʹ� ���Ե��͸� ���̺귯�� ���͸� �߰����� �ٷ� ����߰��� �����ϴ�.
//�������� ������Ʈ ���ø�(VS���)�� �̿��Ͽ� ������Ʈ�� ���� �������� �߰��ؼ� ����ߴ�.
#include <d3d11.h>
#include <d3dcompiler.h> //shader�� �������ϴ� ����� �����Ѵ�.
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
