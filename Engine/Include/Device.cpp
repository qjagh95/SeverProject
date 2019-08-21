#include "stdafx.h"
#include "Device.h"

JEONG_USING

SINGLETON_VAR_INIT(Device);

Device::Device()
	:m_Device(NULLPTR), m_Context(NULLPTR), m_SwapChain(NULLPTR), m_TargerView(NULLPTR), m_DepthView(NULLPTR), m_Hwnd(NULLPTR), m_2DFactory(NULLPTR), m_2DTarget(NULLPTR)
{
}

Device::~Device()
{
	SAFE_RELEASE(m_DepthView);
	SAFE_RELEASE(m_TargerView);
	SAFE_RELEASE(m_SwapChain);

	if (m_Context != NULLPTR)
		m_Context->ClearState();

	SAFE_RELEASE(m_Context);
	SAFE_RELEASE(m_Device);

	SAFE_RELEASE(m_2DFactory);
	SAFE_RELEASE(m_2DTarget);

#ifdef _DEBUG
	//dxgi_debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	//SAFE_RELEASE(dxgi_debug);
	//FreeLibrary(asd);
#endif
}

bool Device::Init(HWND hWnd, unsigned int Width, unsigned int Height, bool isWindowMode)
{
	m_Hwnd = hWnd;
	m_WinSize.Width = Width;
	m_WinSize.Height = Height;

	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3D11_CREATE_DEVICE_DEBUG;

	//asd = LoadLibrary(L"DXGIDebug.dll");
	//auto qwe = reinterpret_cast<HRESULT(*)(REFIID, void**)>(GetProcAddress(asd, "DXGIGetDebugInterface"));
	//qwe(__uuidof(IDXGIDebug), reinterpret_cast<void**>(&dxgi_debug));
	 
#endif
	//DWrite�� 2D������ �������ϴµ� 3D������ ������ �� �ֵ��� �ɼǼ���.
	Flag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL eLevel1 = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL eLevel2 = D3D_FEATURE_LEVEL_11_0;

	//����ü���� �������ø��� ������ �Ѵ�. �׷��� ����۸� �����ϴ� ������ �Ѵ�. �� ������ �̿��Ͽ� ����۸� ������
	DXGI_SWAP_CHAIN_DESC SwapDesc = {};
	SwapDesc.BufferDesc.Width = Width;
	SwapDesc.BufferDesc.Height = Height;
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;					 ///RGBA�ǰ����� ���� 0~1���̷� ����ϰڴ�.
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				 ///����Ʈ�� ������ϴ� ��ġ�������� ���� �̱� �� ����
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; ///�����尪
	SwapDesc.BufferDesc.RefreshRate.Numerator = 60;								 ///���� (//������ֻ���// ������ ����ϴ� �׷���ī�忡�� ����ͷ�)
	SwapDesc.BufferDesc.RefreshRate.Denominator = 1;							 ///�и� (�ʴ� 60���� ���ְڴ�)
	SwapDesc.BufferCount = 1;													 ///������� ����
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						 ///��¿� ����Ÿ��(���� ����� ���)�� ������
	SwapDesc.OutputWindow = hWnd;												 ///���������������??
	SwapDesc.SampleDesc.Count = 1;												 ///��Ƽ�ٸ���̱�� (�������� �Ⱦ�����)
	SwapDesc.SampleDesc.Quality = 0;											 ///���� ���̴��� ����°� ���� (�ϳ����ִ�, ����Ƽ0)
	SwapDesc.Windowed = isWindowMode;											 ///â��� Ǯ��ũ����� ����.
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;								 ///�ɼ� ���������� �ٸ���. �ʿ��ѰͿ� ���� ����.
	//������� ����۸� ����� �Ѱ��̴�.

	//1. ����� Ÿ�� (�׷���ī�尡 2���̻��ϰ��, ������ �׷����� ���� ���⶧���� NULLPTR)
	//2. ����̹� Ÿ�� - �׷���ī�尡 DX11�� �������� ������� CPU�� ���ֵ����ϴ� ���� (DX11�� �������� ������ ������. (���߿� ����̹����ӵ��� ����ϱ�����.)
	//3. HMOUDLE - DirceX�� �����ִ� ������ ����Ʈ��� �ִٸ� ����. == �ǹ̾���
	//4. Flag - �� ����̽��� ������ � �ɼ������� �������ΰ��� or�������� ����ִ´�. (�� �ּ�)
	//5. ���ķ��� - ��ġ�������� (11���� ������ �ȵǸ� ��2��2)
	//6. SDK���� - ������
	//������ ������ ���������� �ּҰ�~
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULLPTR, D3D_DRIVER_TYPE_HARDWARE, 0, Flag, &eLevel1, 1, D3D11_SDK_VERSION, &SwapDesc, &m_SwapChain, &m_Device, &eLevel2, &m_Context)))
	{
		TrueAssert(true);
		return false;
	}

	//����ü���� �������ִ� ����۸� ��º��ձ⿡ ��������Ѵ�. ���̹��۵� ���̹�����Ѵ�.
	//����Ʈ�� ����������Ѵ�.

	ID3D11Texture2D* pBuffer = NULLPTR;										///com��ü�� ������ ���۷���ī��Ʈ�� +1 �����Ѵ�.
	//����ü�ο��� ����۸� �̾ƿ´�.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);	///__uuidof Ŭ������ ���� �ĺ���ȣ�� �˾ƿ��� Ű����
	//����Ÿ�ٺ信 � ���۸� �����س����� �� �信 �����ִ� ���ۿ��� ��������ش�.
	m_Device->CreateRenderTargetView(pBuffer, NULLPTR, &m_TargerView);
	
	SAFE_RELEASE(pBuffer);

	//����� ��ü�� ����ü���� ��������� ���� ���� ���������.	�ű⼭ ����۸� ���� ���̴�.
	//�ȼ������� ���� �ؽ��Ŀ� �����Ѵ�.

	//���� ������ �����Ѵ�. (������� ũ��� �����ϴ�) ���������� �ؽ���2D�� �����Ѵ�.
	D3D11_TEXTURE2D_DESC DepthDesc = {}; 
	DepthDesc.Width = Width;
	DepthDesc.Height = Height;
	DepthDesc.ArraySize = 1;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;			///�� �ؽ��ĸ� ����뵵�� ���̹��۸� ����뵵��.
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;       ///���̿� 24��Ʈ ���Ľǿ� 8��Ʈ�� ����ϰڴ� ��¶�.
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;					
	DepthDesc.MipLevels = 1;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;

	//CPU������ UsageŸ�Կ����� �ٸ���. Default�� CPU���� ���ٺҰ�.

	//Usage
	//1.DEFAULT - �Ϲ������� ���̾��� �ɼ� (�����޸𸮿� �������� �÷�������. �� ������ �����ϴ�. ���Ű���, �޸��б�X)
	//2.IMMUTABLE - ��������� (ó�� ���ý� ��� ������ ���ۿ� �������ϰ� ��¿����θ� ����Ѵ�. ��¼����� �ö󰣴� �����͸� �ƿ� �ٲ����ʰڴٶ�� ����)
	//3.DYNAMIC - �������۸������ �ǽð����� CPU���� �����ؼ� �����͸� �����Ѵ�. (�ַ� ��ƼŬ ��������) (���� - CPU�� ���纻�� ���� �װ��� �����ؼ� ������Ʈ)
	//4.STAGING - ���� ������. (����� ��������. ������ ����� ����.)

	//Texture2D Desc, ä���� �ȼ�����, Texture2D ����
	if (FAILED(m_Device->CreateTexture2D(&DepthDesc, NULLPTR, &pBuffer)))
	{
		TrueAssert(true);
		return false;
	}
	    
	//�ش� ���ۿ� ����-���Ľ� �並 �����.
	m_Device->CreateDepthStencilView(pBuffer, NULLPTR, &m_DepthView);

	SAFE_RELEASE(pBuffer);

	//������� Ÿ�ٺ�� �����並 ������ ���������ο� �����ش�. (Output Merser), ī��Ʈ�� �����͹迭 �� Ÿ�ٺ䰡 �������ϰ�� ���
	m_Context->OMSetRenderTargets(1, &m_TargerView, m_DepthView); 
	//��� Ÿ�ٺ信 ������ ���������ο��� �������ܰ迡 ���Ŀ� �����͵��� ������� ȭ�鿡 ���ٰ��̴�.

	D3D11_VIEWPORT ViewPort = {};
	//����Ʈ�� ȭ��ũ�⸦ �����Ѵ�.
	ViewPort.Width = (float)Width;
	ViewPort.Height = (float)Height;
	ViewPort.MaxDepth = 1;
	//���ؽ�Ʈ�� ����Ʈ�� �����Ѵ�.  (����, �����͹迭) 
	m_Context->RSSetViewports(1, &ViewPort);
	//�����Ͷ������� Depth�Ǵܱ��� ���ؼ� �Ѵ�.

	//DWrite����� ���� �ʱ�ȭ
	//D2DFactory�� �ʱ�ȭ�Ѵ�.
	D2D1_FACTORY_OPTIONS tOption = {};
	tOption.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	//�̱۾������϶� 3D������ �� 2D�������̵���
	//��Ƽ�������϶� �����带 �ϳ� ���� �� 3D�� 2D������ ���ÿ� �ȴ�.
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, tOption, &m_2DFactory)))
		return false;

	// DirectX11 BackBuffer Ÿ���� �̿��Ͽ� 2D ������ Ÿ���� �������ش�.
	IDXGISurface* pBackSurface = NULLPTR;
	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackSurface));

	// 2D ����Ÿ���� �����ϱ� ���� �ɼ� ����
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(pBackSurface, props, &m_2DTarget)))
		return false;

	SAFE_RELEASE(pBackSurface);
	return true;
}

void Device::Clear(float ClearColor[4])
{
	//����ؼ� ȭ���� �����ٰ��̴�.
	m_Context->ClearRenderTargetView(m_TargerView, ClearColor);
	m_Context->ClearDepthStencilView(m_DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Device::Present()
{
	m_SwapChain->Present(0, 0);
}

Vector2 Device::GetWindowToDeviceRatio() const
{
	RECT DeviceSize;
	GetClientRect(m_Hwnd, &DeviceSize);

	return Vector2((float)m_WinSize.Width / (float)DeviceSize.right , (float)m_WinSize.Height/ (float)DeviceSize.bottom);
}
