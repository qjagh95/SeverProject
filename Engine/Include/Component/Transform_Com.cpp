#include "stdafx.h"
#include "Transform_Com.h"
#include "../GameObject.h"

JEONG_USING

Transform_Com::Transform_Com()
{
	m_ComType = CT_TRANSFORM;
	m_isUpdate = true;
	m_isStatic = false;
	m_isZoomMode = false;

	m_DeltaScale.Identity();
	m_DeltaRot.Identity();
	m_DeltaPos.Identity();
}

Transform_Com::Transform_Com(const Transform_Com& copyObject)
	:Component_Base(copyObject)
{
	*this = copyObject;
}

Transform_Com::~Transform_Com()
{
}

bool Transform_Com::Init()
{
	for (int i = 0; i < 3; i++)
	{
		m_LocalAxis[i] = Vector3::Axis[i];
		m_WorldAxis[i] = Vector3::Axis[i];
	}

	m_isZoomMode = RenderManager::Get()->GetIsZoomMode();

	return true;
}

int Transform_Com::Input(float DeltaTime)
{
	return 0;
}
//���⼱ ������ ������Ʈ�� ��� ������.
//������Ʈ�� ���� Transform�� �������ִ�.
int Transform_Com::Update(float DeltaTime)
{
	if (m_isStatic == true)
		return 0;
	else if (m_isUpdate == false)
		return 0;

	m_MatLocal = m_MatLocalScale * m_MatLocalRotation * m_MatWorldPos;

	//����World�� ������ Parent��� ����.
	Matrix Parent;
	Parent.Identity();

	//�ڱ��ڽ��� ��������� ��ȭ�� ������ �־��� �� �÷��׿� ���� ���Ѵ�.
	m_DeltaScale = m_MatWorldScale;
	m_DeltaRot = m_MatWorldRotation;
	m_DeltaPos = m_MatWorldPos;

	if (m_ParentFlag & TPF_SCALE)
	{
		m_DeltaScale *= m_ParentScale;
		Parent *= m_ParentScale;
	}

	if (m_ParentFlag & TPF_ROT)
	{
		m_DeltaRot *= m_ParentRot;
		Parent *= m_ParentRot;
	}

	if (m_ParentFlag & TPF_POS)
	{
		m_DeltaPos *= m_ParentPos;
		Parent *= m_ParentPos;
	}

	// WORLD = �ڱⲨ * �θ𲨴�.
	m_MatWorld = m_MatWorldScale * m_MatWorldRotation * m_MatWorldPos;
	m_MatWorld *= Parent;	//���������� �� �θ���İ� ���Ѵ�. ���� �ڽ��� ���ų� ���� �θ�, �÷��װ� ���ٸ�
							//Identity ������ķ� ���� ���غ��� �ڱ��ڽ��̴�.
	m_isUpdate = false;

	return 0;
}

int Transform_Com::LateUpdate(float DeltaTime)
{
	if (m_isStatic == true)
		return 0;
	else if (m_isUpdate == false)
		return 0;

	m_MatLocal = m_MatLocalScale * m_MatLocalRotation * m_MatWorldPos;

	m_DeltaScale = m_MatWorldScale;
	m_DeltaRot = m_MatWorldRotation;
	m_DeltaPos = m_MatWorldPos;

	Matrix Parent;
	Parent.Identity();

	if (m_ParentFlag & TPF_SCALE)
	{
		m_DeltaScale *= m_ParentScale;
		Parent *= m_ParentScale;
	}

	if (m_ParentFlag & TPF_ROT)
	{
		m_DeltaRot *= m_ParentRot;
		Parent *= m_ParentRot;
	}

	if (m_ParentFlag & TPF_POS)
	{
		m_DeltaPos *= m_ParentPos;
		Parent *= m_ParentPos;
	}

	// WORLD = �ڱⲨ * �θ𲨴�.
	m_MatWorld = m_MatWorldScale * m_MatWorldRotation * m_MatWorldPos;
	m_MatWorld *= Parent;
	//World = S R T����.

	return 0;
}

void Transform_Com::Collision(float DeltaTime)
{
}

void Transform_Com::Render(float DeltaTime)
{
	m_DeltaMove = Vector3::Zero;
}

Transform_Com * Transform_Com::Clone()
{
	return new Transform_Com(*this);
}

void Transform_Com::SetLocalScale(const Vector3 & vScale)
{
	//ũ�Ⱚ�� �޴´�
	m_LocalScale = vScale;
	//��Ŀ� ũ�Ⱚ�� ���Ѵ�
	m_MatLocalScale.Scaling(m_LocalScale);

	m_isUpdate = true;
}

void Transform_Com::SetLocalScale(float x, float y, float z)
{
	//ũ�Ⱚ�� �޴´�
	m_LocalScale = Vector3(x, y, z);
	//��Ŀ� ũ�Ⱚ�� ���Ѵ�
	m_MatLocalScale.Scaling(m_LocalScale);

	m_isUpdate = true;
}

void Transform_Com::SetLocalRotation(const Vector3 & vRot)
{
	//ȸ������ �޴´�
	m_LocalRotation = vRot;
	//ȸ����Ŀ� �����ش�
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void Transform_Com::SetLocalRotation(float x, float y, float z)
{
	//ȸ������ �޴´�
	m_LocalRotation = Vector3(x, y, z);
	//ȸ����Ŀ� �����ش�
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void Transform_Com::SetLocalRotX(float x)
{
	//ȸ������ �޴´�
	m_LocalRotation.x = x;
	//ȸ����Ŀ� �����ش�
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void Transform_Com::SetLocalRotY(float y)
{
	//ȸ������ �޴´�
	m_LocalRotation.y = y;
	//ȸ����Ŀ� �����ش�
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void Transform_Com::SetLocalRotZ(float z)
{
	//ȸ������ �޴´�
	m_LocalRotation.z = z;
	//ȸ����Ŀ� �����ش�
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void Transform_Com::SetLocalPos(const Vector3 & vPos)
{
	m_LocalPos = vPos;

	m_MatLocalPos.Translation(m_LocalPos);

	m_isUpdate = true;
}

void Transform_Com::SetLocalPos(float x, float y, float z)
{
	m_LocalPos = Vector3(x, y, z);

	m_MatLocalPos.Translation(m_LocalPos);

	m_isUpdate = true;
}

//�� ���� ������ �˾ƿ´�.
void Transform_Com::ComputeLocalAxis()
{
	for (int i = 0; i < 3; ++i)
	{
		//��� ���Լ�.
		m_LocalAxis[i] = Vector3::Axis[i].TransformNormal(m_MatLocalRotation);
		//ũ��1���ͷ� ���� ���Ⱚ�� ������ ����.
		m_LocalAxis[i].Nomallize();
	}
}

void Transform_Com::SetWorldScale(const Vector3 & vScale)
{
	//ũ�Ⱚ�� �޴´�
	m_WorldScale = vScale;
	//��Ŀ� ũ�Ⱚ�� ���Ѵ�
	m_MatWorldScale.Scaling(m_WorldScale);

	m_isUpdate = true;
}

void Transform_Com::SetWorldScale(float x, float y, float z)
{
	//ũ�Ⱚ�� �޴´�
	m_WorldScale = Vector3(x, y, z);
	//��Ŀ� ũ�Ⱚ�� ���Ѵ�
	m_MatWorldScale.Scaling(m_WorldScale);

	m_isUpdate = true;
}
void Transform_Com::AddScale(float Scale)
{	
	//ũ�Ⱚ�� �޴´�
	m_WorldScale += Vector3(Scale, Scale, 1.0f);
	//��Ŀ� ũ�Ⱚ�� ���Ѵ�
	m_MatWorldScale.Scaling(m_WorldScale);

	m_isUpdate = true;
}

void Transform_Com::SetWorldRotX(float x)
{
	//ȸ������ �޴´�
	m_WorldRotation.x = x;
	//ȸ����Ŀ� �����ش�
	m_MatWorldRotation.Rotation(m_WorldRotation);

	ComputeWorldAxis();

	m_isUpdate = true;
}

void Transform_Com::SetWorldRotY(float y)
{
	//ȸ������ �޴´�
	m_WorldRotation.y = y;
	//ȸ����Ŀ� �����ش�
	m_MatWorldRotation.Rotation(m_WorldRotation);

	ComputeWorldAxis();

	m_isUpdate = true;
}

void Transform_Com::SetWorldRotZ(float z)
{
	//ȸ������ �޴´�
	m_WorldRotation.z = z;
	//ȸ����Ŀ� �����ش�
	m_MatWorldRotation.Rotation(m_WorldRotation);

	ComputeWorldAxis();

	m_isUpdate = true;
}

void Transform_Com::SetWorldPos(const Vector3 & vPos)
{
	m_DeltaMove = vPos - m_WorldPos;
	m_WorldPos = vPos;

	m_MatWorldPos.Translation(m_WorldPos);

	m_isUpdate = true;
}

void Transform_Com::SetWorldPos(float x, float y, float z)
{
	m_DeltaMove = Vector3(x, y, z) - m_WorldPos;
	m_WorldPos = Vector3(x, y, z);

	m_MatWorldPos.Translation(m_WorldPos);

	m_isUpdate = true;
}

void Transform_Com::SetWorldPivot(const Vector3& vPos)
{
	m_Pivot = vPos;
}
void Transform_Com::SetWorldPivot(float x, float y, float z)
{
	m_Pivot = Vector3(x, y, z);
}

void Transform_Com::Move(AXIS eAxis, float Speed)
{
	Move(m_WorldAxis[eAxis] * Speed);
}

void Transform_Com::Move(AXIS eAxis, float Speed, float DeltaTime)
{
	Move(m_WorldAxis[eAxis] * Speed * DeltaTime);

	switch (eAxis)
	{
	case JEONG::AXIS_X:
		m_Dir.x = Speed > 0.0f ? 1.0f : -1.0f;
		break;
	case JEONG::AXIS_Y:
		m_Dir.y = Speed > 0.0f ? 1.0f : -1.0f;
		break;
	case JEONG::AXIS_Z:
		break;
	}
}

void Transform_Com::Move(const Vector3 & vDir, float Speed)
{
	Move(vDir * Speed);
}

void Transform_Com::Move(const Vector3 & vDir, float Speed, float DeltaTime)
{
	Move(vDir * Speed * DeltaTime);
}

void Transform_Com::Move(const Vector3 & vMove)
{
	m_WorldPos += vMove;
	m_DeltaMove = vMove;

	m_MatWorldPos.Translation(m_WorldPos);

	m_isUpdate = true;
}

void Transform_Com::RotationX(float x)
{
	Rotation(Vector3(x, 0.0f, 0.0f));
}

void Transform_Com::RotationX(float x, float DeltaTime)
{
	Rotation(Vector3(x * DeltaTime, 0.0f, 0.0f));
}

void Transform_Com::RotationY(float y)
{
	Rotation(Vector3(0.0f, y, 0.0f));
}

void Transform_Com::RotationY(float y, float DeltaTime)
{
	Rotation(Vector3(0.0f, y * DeltaTime, 0.0f));
}

void Transform_Com::RotationZ(float z)
{
	Rotation(Vector3(0.0f, 0.0f, z));
}

void Transform_Com::RotationZ(float z, float DeltaTime)
{
	//�����̼��Լ����� += ���ش�.
	Rotation(Vector3(0.0f, 0.0f, z * DeltaTime));
}

void Transform_Com::Rotation(const Vector3 & vRot, float DeltaTime)
{
	Rotation(vRot * DeltaTime);
}

void Transform_Com::Rotation(const Vector3 & vRot)
{
	m_WorldRotation += vRot;
	m_MatWorldRotation.Rotation(m_WorldRotation);
	//ȸ����İ������� �� WorldAxis���� ��ȯ�Ѵ�.
	ComputeWorldAxis();
	m_isUpdate = true;
}

//�� ���� ������ �˾ƿ´�.
void Transform_Com::ComputeWorldAxis()
{
	for (int i = 0; i < 3; ++i)
	{
		//������ ��Ʈ����(ȸ�����)�� ���� ���͸� ��ȯ�Ѵ�.
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(m_MatWorldRotation);
		//ũ��1���ͷ� ���� ���Ⱚ�� ������ ����.
		m_WorldAxis[i].Nomallize();
	}
}

void Transform_Com::LookAt(GameObject * object, AXIS eAxis)
{
	LookAt(object->GetTransform()->GetWorldPos(), eAxis);
}

void Transform_Com::LookAt(Component_Base * component, AXIS eAxis)
{
	LookAt(component->GetTransform()->GetWorldPos(), eAxis);
}

void Transform_Com::LookAt(const Vector3 & Vec, AXIS eAxis)
{
	//�ٶ󺸷��� ������ ���Ѵ�. (���ͻ��� = �ٶ󺸴¹���) 
	Vector3 View = Vec - m_WorldPos;
	View.Nomallize();

	//�������� �����Ѵ�.
	Vector3 Axis = Vector3::Axis[eAxis];

	//������� �ٶ󺸷��� ����(View)�� ������ ���Ѵ� (����)
	float Angle = Axis.GetAngle(View);

	Vector3 vRotAxis = Axis.Cross(View);
	vRotAxis.Nomallize();

	//���� �࿡ ���� ȸ����� ����.
	m_MatWorldRotation.RotationAxis(Angle, vRotAxis);
	ComputeWorldAxis();

	m_isUpdate = true;
}

float Transform_Com::GetAngle(GameObject * Target)
{
	return GetAngle(Target->GetTransform());
}

float Transform_Com::GetAngle(Transform_Com * Target)
{
	return m_WorldPos.GetAngle(Target->GetWorldPos());
}

void Transform_Com::SetParentFlag(int Flag)
{
	m_ParentFlag = Flag;
}

void Transform_Com::AddParentFlag(TRANSFORM_PARENT_FLAG Flag)
{
	m_ParentFlag |= Flag;
}

void Transform_Com::DeleteParentFlag(TRANSFORM_PARENT_FLAG Flag)
{
	if (m_ParentFlag & Flag)
		m_ParentFlag ^= Flag; //xor0�϶� 1 , 1�϶� 0
}

void Transform_Com::DeleteParentFlag()
{
	m_ParentFlag = 0;
}

void Transform_Com::SetParentPos(const Matrix& parentPos)
{
	m_ParentPos = parentPos;
}
void Transform_Com::SetParentRot(const Matrix& parentRot)
{
	m_ParentRot = parentRot;
}

void Transform_Com::SetParentScale(const Matrix& parentScale)
{
	m_ParentScale = parentScale;
}

Matrix Transform_Com::GetParentPos() const
{
	return m_ParentPos;
}

Matrix Transform_Com::GetParentRot() const
{
	return m_ParentRot;
}

Matrix Transform_Com::GetParentScale() const
{
	return m_ParentScale;
}
