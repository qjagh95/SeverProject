#pragma once
JEONG_BEGIN

class Shader;
class JEONG_DLL ShaderManager
{
public:
	bool Init();
	bool LoadShader(const string& KeyName, const TCHAR* FileName, char* Entry[ST_MAX], const string& PathKey = SHADER_PATH);
	//1.���̴����� �ø�ƽ �̸� 2. �ε��� 3. ���� 4. ����Ʈ������ 5.����Ʈ�� ����
	void AddInputElement(char* Semantic, int Index, DXGI_FORMAT Format, int Size, int InputSlot = 0, D3D11_INPUT_CLASSIFICATION eInputClass = D3D11_INPUT_PER_VERTEX_DATA, int InstanceStepRate = 0);
	bool CreateInputLayOut(const string& InputLayoutKey, const string& ShaderKey);
	bool CreateCBuffer(const string& KeyName, int BufferSize, int RegisterNumber,int ShaderType);
	bool UpdateCBuffer(const string& KeyName, void *Info);

	CBuffer* FindCBuffer(const string& KeyName);
	Shader* FindShader(const string& KeyName);
	ID3D11InputLayout* FindInputLayOut(const string& KeyName);

private:
	unordered_map<string, Shader*> m_ShaderMap;
	//InputLayOut = VertexShader�� Vertex������ �����ϴ� ����� ����ִ�.
	unordered_map<string, ID3D11InputLayout*> m_LayOutMap;
	//InputLayout�� ��� �������� �𸣴� vector (�ø�ƽ)
	vector<D3D11_INPUT_ELEMENT_DESC> m_vecInputElement;
	//���̴��� ������ ���۰���
	unordered_map<string, CBuffer*> m_CBufferMap;

	int m_InputLayOutSize;

public:
	CLASS_IN_SINGLE(ShaderManager)
};

JEONG_END

