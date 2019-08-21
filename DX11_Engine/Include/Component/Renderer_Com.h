#pragma once
#include "Component_Base.h"
JEONG_BEGIN

class Mesh;
class Shader;
class Material_Com;
class RenderState;
class JEONG_DLL Renderer_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Renderer_Com* Clone() override;
	void AfterClone() override;

public:
	void SetMesh(Mesh* mesh);
	void SetMesh(const string& KeyName);
	void SetShader(Shader* shader);
	void SetShader(const string& KeyName);
	void SetLayOut(const string& KeyName);
	void SetRenderState(const string& KeyName);
	void SetScreenRender(bool Value) { m_isScreenRender = Value; }

	bool CreateRendererCBuffer(const string& KeyName, int BufferSize);
	void UpdateRendererCBuffer(const string& KeyName, void* pData, int BufferSize);

	void CheckComponent();
	void DeleteComponentCBuffer(Component_Base* DeleteCom);

private:
	void UpdateTransform();
	RendererCBuffer* FindCBuffer(const string& KeyName);

private:
	//���� ������Ʈ�� �Ž�, ���̴�, ���������� �����ִ´�.
	//�⺻���� �ʱ�ȭ�������� �ٷ� White(1, 1, 1, 1) = ���ϸ� �������� ������ �ִ�.
	Mesh* m_Mesh;				
	Shader* m_Shader;
	Material_Com* m_Material;
	ID3D11InputLayout* m_LayOut;
	RenderState* m_RenderState[RS_END];
	ComponentCBuffer m_ComponentCBuffer;
	bool m_isScreenRender;

	unordered_map<string, RendererCBuffer*> m_CBufferMap;

private:
	Renderer_Com();
	Renderer_Com(const Renderer_Com& copyObject);
	~Renderer_Com();

public:
	friend class GameObject;
};

JEONG_END

