#include "stdafx.h"
#include "ShaderManager.h"
#include "Shader.h"

#include "../Device.h"

JEONG_USING
SINGLETON_VAR_INIT(ShaderManager)

ShaderManager::ShaderManager() : m_InputLayOutSize(0)
{
}

ShaderManager::~ShaderManager()
{
	Safe_Release_Map(m_ShaderMap);
	Safe_Release_Map(m_LayOutMap);

	unordered_map<string, CBuffer*>::iterator StartIter = m_CBufferMap.begin();
	unordered_map<string, CBuffer*>::iterator EndIter = m_CBufferMap.end();
	for (;StartIter != EndIter; StartIter++)
	{
		SAFE_RELEASE(StartIter->second->cBuffer);
		SAFE_DELETE(StartIter->second);
	}

	m_CBufferMap.clear();
}

bool ShaderManager::Init()
{
	char* Entry[ST_MAX] = {};

	//���̴����� �Լ� �̸�.
	Entry[ST_VERTEX] = "Standard_Color_VS";
	Entry[ST_PIXEL] = "Standard_Color_PS";
	//���̴����� �ε�
	if (LoadShader(STANDARD_COLOR_SHADER, TEXT("Standard.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

	//�ø�ƽ �̸� �߰�
	AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
	//InputLayout����
	if (CreateInputLayOut(POS_COLOR_LAYOUT, STANDARD_COLOR_SHADER) == false)
	{
		TrueAssert(true);
		return false;
	}

	Entry[ST_VERTEX] = "Standard_UV_VS";
	Entry[ST_PIXEL] = "Standard_UV_PS";

	if (LoadShader(STANDARD_UV_SHADER, TEXT("Standard.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

	AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 8);

	if (CreateInputLayOut(POS_UV_LAYOUT, STANDARD_UV_SHADER) == false)
	{
		TrueAssert(true);
		return false;
	}

	Entry[ST_VERTEX] = "UIVS";
	Entry[ST_PIXEL] = "UIPS";

	if (LoadShader(BUTTON_SHADER, TEXT("UIShader.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

	Entry[ST_VERTEX] = "BarVS";
	Entry[ST_PIXEL] = "BarPS";

	if (LoadShader(BAR_SHADER, TEXT("BarShader.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

	Entry[ST_VERTEX] = "CheckBoxVS";
	Entry[ST_PIXEL] = "CheckBoxPS";

	if (LoadShader(CHECKBOX_SHADER, TEXT("CheckBoxShader.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

//#ifdef _DEBUG
	Entry[ST_VERTEX] = "ColliderVS";
	Entry[ST_PIXEL] = "ColliderPS";

	if (LoadShader(COLLIDER_SHADER, TEXT("Collider.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

	AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);

	if (CreateInputLayOut(POS_LAYOUT, COLLIDER_SHADER) == false)
	{
		TrueAssert(true);
		return false;
	}

	Entry[ST_VERTEX] = "TileVS";
	Entry[ST_PIXEL] = "TilePS";

	if (LoadShader(TILE_SHADER, TEXT("TileShader.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

//#endif

	Entry[ST_VERTEX] = "FullScreenVS";
	Entry[ST_PIXEL] = "FullScreenPS";
	if (LoadShader(FULLSCREEN_SHADER, TEXT("Standard.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

	Entry[ST_VERTEX] = "StandardTexStaticVS";
	Entry[ST_PIXEL] = "StandardTexStaticPS";
	if (LoadShader(STANDARD_UV_STATIC_SHADER, TEXT("Standard.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

	Entry[ST_VERTEX] = "BackColorVS";
	Entry[ST_PIXEL] = "BackColorPS";
	if (LoadShader("BackColorShader", TEXT("BackColorShader.fx"), Entry) == false)
	{
		TrueAssert(true);
		return false;
	}

	//������� Create
	CreateCBuffer("Transform", sizeof(TransformCBuffer), 0, CST_VERTEX | CST_PIXEL);
	CreateCBuffer("Material", sizeof(MaterialCbuffer), 1, CST_VERTEX | CST_PIXEL);
	CreateCBuffer("Component", sizeof(ComponentCBuffer), 2, CST_VERTEX | CST_PIXEL);
	CreateCBuffer("Collider", sizeof(Vector4), 8, CST_VERTEX | CST_PIXEL);


	return true;
}

bool ShaderManager::LoadShader(const string & KeyName, const TCHAR * FileName, char * Entry[ST_MAX], const string & PathKey)
{
	Shader* newShader = FindShader(KeyName);

	if (newShader != NULLPTR)
	{
		SAFE_RELEASE(newShader);
		return true;
	}

	newShader = new Shader();

	if (newShader->LoadShader(KeyName, FileName, Entry, PathKey) == false)
	{
		SAFE_RELEASE(newShader);
		return false;
	}

	m_ShaderMap.insert(make_pair(KeyName, newShader));

	return true;
}

void ShaderManager::AddInputElement(char * Semantic, int Index, DXGI_FORMAT Format, int Size, int InputSlot, D3D11_INPUT_CLASSIFICATION eInputClass, int InstanceStepRate)
{
	D3D11_INPUT_ELEMENT_DESC elementDesc;
	elementDesc.SemanticName = Semantic;				///���̴� ���Ͽ��ִ� �ø�ƽ �̸�
	elementDesc.SemanticIndex = Index;					///�ø�ƽ ���ʿ� ���ڸ� �ٿ��� �� �ִµ� �� ����
	elementDesc.AlignedByteOffset = m_InputLayOutSize;	///GPU���� �� ��Ұ� �޸𸮿��� ������������ ��ŭ�������ֳ�
	elementDesc.InputSlot = InputSlot;					///�Ⱦ����ߵ���Ʈ��
	elementDesc.InputSlotClass = eInputClass;			///�Ⱦ����ߵ���Ʈ��
	elementDesc.InstanceDataStepRate = InstanceStepRate;///�Ⱦ����ߵ���Ʈ��
	elementDesc.Format = Format;						///����R32��Ʈ...G32��Ʈ...

	m_InputLayOutSize += Size;

	m_vecInputElement.push_back(elementDesc);
}

bool ShaderManager::CreateInputLayOut(const string & InputLayoutKeyName, const string & ShaderKey)
{
	Shader* getShader = FindShader(ShaderKey);

	if (getShader == NULLPTR)
		return false;

	ID3D11InputLayout* newLayout = FindInputLayOut(InputLayoutKeyName);

	if (newLayout != NULLPTR)
	{
		TrueAssert(true);
		return false;
	}

	if (FAILED(Device::Get()->GetDevice()->CreateInputLayout(&m_vecInputElement[0], (UINT)m_vecInputElement.size(), getShader->GetVertexShaderCode(), getShader->GetVertexShaderCodeSize(), &newLayout)))
	{
		TrueAssert(true);
		return false;
	}

	SAFE_RELEASE(getShader);

	m_InputLayOutSize = 0;
	m_vecInputElement.clear();

	m_LayOutMap.insert(make_pair(InputLayoutKeyName, newLayout));

	return true;
}

bool ShaderManager::CreateCBuffer(const string & KeyName, int BufferSize, int RegisterNumber, int ShaderType)
{
	CBuffer* newCBuffer = FindCBuffer(KeyName);

	if (newCBuffer != NULLPTR)
		return false;

	newCBuffer = new CBuffer();
	newCBuffer->BufferSize = BufferSize;
	newCBuffer->ShaderType = ShaderType;
	newCBuffer->RegisterNumber = RegisterNumber;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = BufferSize;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(Device::Get()->GetDevice()->CreateBuffer(&bufferDesc, NULLPTR, &newCBuffer->cBuffer)))
		return false;

	m_CBufferMap.insert(make_pair(KeyName, newCBuffer));

	return true;
}

//���ؽ��� �ȼ����̴��� ������۸� �����Ѵ�.
bool ShaderManager::UpdateCBuffer(const string& KeyName, void * Info)
{
	CBuffer* getBuffer = FindCBuffer(KeyName);

	if (getBuffer == NULLPTR)
		return false;

	D3D11_MAPPED_SUBRESOURCE mapDesc = {};

	Device::Get()->GetContext()->Map(getBuffer->cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapDesc);
	{
		memcpy(mapDesc.pData, Info, getBuffer->BufferSize);
	}
	Device::Get()->GetContext()->Unmap(getBuffer->cBuffer, 0);

	if (getBuffer->ShaderType & CST_VERTEX)
		Device::Get()->GetContext()->VSSetConstantBuffers(getBuffer->RegisterNumber, 1, &getBuffer->cBuffer);//������ġ, ��ȣ
	if(getBuffer->ShaderType & CST_PIXEL)
		Device::Get()->GetContext()->PSSetConstantBuffers(getBuffer->RegisterNumber, 1, &getBuffer->cBuffer);//������ġ, ��ȣ

	return true;
}

CBuffer * ShaderManager::FindCBuffer(const string & KeyName)
{
	unordered_map<string, CBuffer*>::iterator FindIter = m_CBufferMap.find(KeyName);

	if (FindIter == m_CBufferMap.end())
		return NULLPTR;

	return FindIter->second;
}

Shader * ShaderManager::FindShader(const string & KeyName)
{
	unordered_map<string, Shader*>::iterator FindIter = m_ShaderMap.find(KeyName);

	if (FindIter == m_ShaderMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}

ID3D11InputLayout * ShaderManager::FindInputLayOut(const string & KeyName)
{
	unordered_map<string, ID3D11InputLayout*>::iterator FindIter = m_LayOutMap.find(KeyName);

	if (FindIter == m_LayOutMap.end())
		return NULLPTR;

	return FindIter->second;
}
