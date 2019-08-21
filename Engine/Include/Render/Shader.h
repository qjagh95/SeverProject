#pragma once
#include "../RefCount.h"

JEONG_BEGIN

class JEONG_DLL Shader : public RefCount
{
public:
	bool LoadShader(const string& shaderName, const TCHAR* FileName, char* Entry[ST_MAX], const string& PathKey = SHADER_PATH);
	void SetShader();

	void* GetVertexShaderCode() const { return m_VsBlob->GetBufferPointer(); }
	size_t GetVertexShaderCodeSize() const { return m_VsBlob->GetBufferSize(); }

private:
	ID3DBlob* m_VsBlob;			///Blop = ���̴� �ڵ带 �����ϴ� ����.
	ID3DBlob* m_PsBlob;			///Blop = ���̴� �ڵ带 �����ϴ� ����.
	ID3D11VertexShader* m_Vs;
	ID3D11PixelShader* m_Ps;  
	string m_ShaderName;

private:
	bool LoadVertexShader(const TCHAR* FileName, char* Entry, const string& PathKey = SHADER_PATH);
	bool LoadPixelShader(const TCHAR* FileName, char* Entry, const string& PathKey = SHADER_PATH);

private:
	Shader();
	~Shader();

public:
	friend class ShaderManager;
};

JEONG_END