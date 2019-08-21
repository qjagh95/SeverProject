#include "stdafx.h"
#include "Shader.h"
#include "../PathManager.h"
#include "../Device.h"

JEONG_USING

Shader::Shader()
 : m_VsBlob(NULLPTR), m_PsBlob(NULLPTR), m_Vs(NULLPTR), m_Ps(NULLPTR)
{	
}

Shader::~Shader()
{
	SAFE_RELEASE(m_VsBlob);
	SAFE_RELEASE(m_PsBlob);
	SAFE_RELEASE(m_Vs);
	SAFE_RELEASE(m_Ps);
}

void Shader::SetShader()
{
	Device::Get()->GetContext()->VSSetShader(m_Vs, NULLPTR, 0);
	Device::Get()->GetContext()->PSSetShader(m_Ps, NULLPTR, 0);
}

bool Shader::LoadShader(const string & shaderName, const TCHAR * FileName, char * Entry[ST_MAX], const string & PathKey)
{
	m_ShaderName = shaderName;
	SetTag(shaderName);

	if (LoadVertexShader(FileName, Entry[ST_VERTEX], PathKey) == false)
		return false;

	if (LoadPixelShader(FileName, Entry[ST_PIXEL], PathKey) == false)
		return false;

	return true;
}

bool Shader::LoadVertexShader(const TCHAR * FileName, char * Entry, const string & PathKey)
{
	const TCHAR* pPath = PathManager::Get()->FindPath(PathKey);

	wstring	FullPath;

	if (pPath != NULLPTR)
		FullPath = pPath;
	FullPath += FileName;

	DWORD Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif 
	//���̴� ������ �� �����޼����� �������� ����.
	ID3DBlob* ErrorMessage = NULLPTR;

	//HLSL�� �������Ѵ�.
	if (FAILED(D3DCompileFromFile(FullPath.c_str(), NULLPTR, D3D_COMPILE_STANDARD_FILE_INCLUDE, Entry, "vs_5_0", Flag, 0, &m_VsBlob, &ErrorMessage)))
	{
		OutputDebugStringA((char*)ErrorMessage->GetBufferPointer());
		TrueAssert(true);
		return false;
	}

	//Blob�� �����ϵ� HLSL�ڵ带 ����Ͽ� ���ؽ����̴� ��ü�� ������.
	if (FAILED(Device::Get()->GetDevice()->CreateVertexShader(m_VsBlob->GetBufferPointer(), m_VsBlob->GetBufferSize(), NULLPTR, &m_Vs)))
	{
		TrueAssert(true);
		return false;
	}

	return true;
}

bool Shader::LoadPixelShader(const TCHAR * FileName, char * Entry, const string & PathKey)
{
	const TCHAR* pPath = PathManager::Get()->FindPath(PathKey);

	wstring	FullPath;

	if (pPath != NULLPTR)
		FullPath = pPath;
	FullPath += FileName;

	DWORD Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif
	//���̴� ������ �� �����޼����� �������� ����.
	ID3DBlob* ErrorMessage = NULLPTR;

	//HLSL�� �������Ѵ�.
	if (FAILED(D3DCompileFromFile(FullPath.c_str(), NULLPTR, D3D_COMPILE_STANDARD_FILE_INCLUDE, Entry, "ps_5_0", Flag, 0, &m_PsBlob, &ErrorMessage)))
	{
		OutputDebugStringA((char*)ErrorMessage->GetBufferPointer());
		return false;
	}

	//Blob�� �����ϵ� HLSL�ڵ带 ����Ͽ� ���ؽ����̴� ��ü�� ������.
	if (FAILED(Device::Get()->GetDevice()->CreatePixelShader(m_PsBlob->GetBufferPointer(), m_PsBlob->GetBufferSize(), NULLPTR, &m_Ps)))
		return false;

	return true;
}