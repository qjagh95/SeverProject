#pragma once
JEONG_BEGIN

class JEONG_DLL Device
{
public:
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height, bool isWindowMode);
	void Clear(float ClearColor[4]);
	void Present();
	ID3D11Device* GetDevice() const { return m_Device; }
	ID3D11DeviceContext* GetContext() const { return m_Context; }
	IDXGISwapChain* GetSwapChain() const { return m_SwapChain; }
	WinSize GetWinSize() const { return m_WinSize; }
	Vector2 GetWindowToDeviceRatio() const;
	ID2D1RenderTarget* Get2DRenderTarget() const { return m_2DTarget; }

private:
	//Com - Component Object�� ���� - ���� RefCount������� ���ư���.
	//Device�����͵��� Com(IUNOWS)�� ��ӹ޴´�(RefCount)

	//DX�� ������۸������ �������ø����� ����Ѵ�.
	//�������ø��� - ����ۿ� �׸���->���۹��� �� ǥ���̵ǰ� �� ǥ���� ����۰��ȴ�.��� �Դٰ����ϴ� ���. (����Ʈ�� �����Ϳ���� ���)
	//�����ϴ� �۾��� ���⶧���� �� ������ (API�� BitBlt - �ȼ��� ����)

	//11�� ��Ƽ�����巣������ �����ϴٴ����� ������.
	//��Ƽ������ - ����ó�����α׷��� 1���Լ����� 2���Լ����� ����

	//11���ʹ� ����̽� ���ؽ�Ʈ�� �����ִ�

	//1. �̵̹�������ؽ�Ʈ - �Ϲ������� ���(��ö�¶�)
	//2. ���۵����ؽ�Ʈ - ��Ƽ������

	//9�� ����̽��� ���� ����ϴٺ��� 11���ʹ� ����
	//����̽��� ���� ���ۻ��� ���ؽ�Ʈ�� �̿��Ͽ� ���¼������Ѵ�.

	//����ü�� - ����۸� �������ٳ�(�����츦 �ø��� ����ü���� �þ�� �̰��� Ȱ���ϸ� ����ȭ��, ����ȭ��, �ĸ�ȭ�� ���� �� �� �ִ�)

	//�ݵ�� WinSize�� DX����̽� ����� ���� �ʿ�� ����.
	//Window���� ���� ���� ���� �� �� ���� �׸��ٰ� �����ϸ� �ȴ�.

	ID3D11Device* m_Device;					///����̽� - �׷���ī��� �����Ͽ� ����� ��� �����ִ� ��.
	ID3D11DeviceContext* m_Context;			///����̽� ���ؽ�Ʈ - ����̽��� ���¼����� ����Ѵ�
	IDXGISwapChain* m_SwapChain;			///����ü�� - ����۸� �����Ѵ�.
	ID3D11RenderTargetView* m_TargerView;   ///����Ÿ�ٺ� - ����� ����Ÿ�� (����̽��� ����س��� ��) �׳� ���۹�
	ID3D11DepthStencilView* m_DepthView;    ///���̽��ĽǺ�
	WinSize m_WinSize;
	HWND m_Hwnd;

#ifdef _DEBUG
	//IDXGIDebug* dxgi_debug;
	//HMODULE asd;
#endif

	//2D Interface
	ID2D1RenderTarget* m_2DTarget;
	ID2D1Factory* m_2DFactory;

public:
	CLASS_IN_SINGLE(Device)
};

JEONG_END
