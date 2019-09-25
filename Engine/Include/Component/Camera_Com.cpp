#include "stdafx.h"
#include "Camera_Com.h"
#include "../GameObject.h"
#include "Transform_Com.h"

JEONG_USING

Camera_Com::Camera_Com()
{
	m_ComType = CT_CAMERA;
	m_CameraType = CT_PERSPECTIVE;
	m_Target = NULLPTR;
}

Camera_Com::~Camera_Com()
{
}

Camera_Com::Camera_Com(const Camera_Com & camera)
	:Component_Base(camera)
{
	m_CameraType = camera.m_CameraType;
	m_View = camera.m_View;
	m_Projection = camera.m_Projection;
	m_Target = NULLPTR;
}

bool Camera_Com::Init()
{
	m_WinSize = Vector3(1280.0f, 720.0f, 1.0f);
	m_MaxPos = Vector3(50000.0f, 50000.0f, 1.0f);
	return true;
}

int Camera_Com::Input(float DeltaTime)
{
	return 0;
}

int Camera_Com::Update(float DeltaTime)
{
	m_View.Identity();

	Vector3 WorldPos = m_Transform->GetWorldPos();

	if (m_Target != NULLPTR)
	{
		Vector3 TargetPos = m_Target->GetTransform()->GetWorldPos();
		Vector3 Dir = m_Target->GetTransform()->GetDir();

		Vector3 Lerp = Vector3::Lerp(WorldPos, Vector3((TargetPos.x - m_WinSize.x * 0.6f), (TargetPos.y - m_WinSize.y * 0.2f), -1.0f), DeltaTime * 2.0f);

		if (Lerp.x < 0.0f)
			Lerp.x = 0.0f;
		else if (Lerp.x + m_WinSize.x >= m_MaxPos.x)
			Lerp.x = m_MaxPos.x - m_WinSize.x;

		if (Lerp.y < 0.0f)
			Lerp.y = 0.0f;
		else if (Lerp.y + m_WinSize.y >= m_MaxPos.y)
			Lerp.y = m_MaxPos.y - m_WinSize.y;

		m_Transform->SetWorldPos(Lerp);

		Vector3 Temp = m_Transform->GetWorldPos();
		Temp *= -1.0f;
		memcpy(&m_View[3][0], &Temp, sizeof(Vector3));
	}

	return 0;
}

int Camera_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Camera_Com::Collision(float DeltaTime)
{
}

void Camera_Com::Render(float DeltaTime)
{
}

Camera_Com * Camera_Com::Clone()
{
	return new Camera_Com(*this);
}

void Camera_Com::SetCameraType(CAMERA_TYPE eType)
{
	m_CameraType = eType;

	switch (eType)
	{
		case CT_PERSPECTIVE:
			m_Projection = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle), m_Width / m_Height, m_Near, m_Far);
			break;

		case CT_ORTHO:
			/*
			화면을 800 600이라고 가정한다. 스케일값이 100이라고 가정한다.

			(버텍스 데이터)     (월드행렬)
			 0,    0.5, 0, 1	100 0   0 0
			 0.5, -0.5, 0, 1	0   100 0 0
			-0.5, -0.5, 0, 1	0   0   1 0
								100 100 0 1

			(100) = (0 0 * 0) + (1 0 * 1) + (2 0 * 3) + (3 0 * 3)
			(150) = (0 1 * 0) + (1 1 * 1) + (2 1 * 3) + (3 1 * 3)

			위에서 월드행렬에 곱한 버텍스데이터를 직교투영행렬공식에 변환과정.
			100, 150, 0, 1		1/400 0      0 0
			150, 50,  0, 1		0     1/-300 0 0
			50,  50,  0, 1		0     0      1 0
			-1,   1,  1, 1

			곱한 값
			-0.75, 0.5
			-0.625, 0.83

			직교투영 공식
			2/(right-left)      0					0            0
			0					2/(top-bottom)      0            0
			0					0					1/(zf-zn)	 0
			(left+right)/(left-right)  (top+bottom)/(bottom-top)  zn/(zn-zf)
			*/

			//여기서 Pos가 뒤집어진 결과가 나온다. 
			//그래서 인덱스를 반대로 돌려줘야 정상적인 출력이 가능하다. (좌상단으로 좌표를 잡았을경우)
			m_Projection = XMMatrixOrthographicOffCenterLH(0.0f, m_Width, 0.0f, m_Height, m_Near, m_Far);
			break;
			//투영공식을 위 공식을 통하여 내보내겠다는 뜻.
	}
}

void Camera_Com::SetCameraInfo(CAMERA_TYPE eType, float Width, float Height, float ViewAngle, float Near, float Far)
{
	m_Width = Width;
	m_Height = Height;
	m_Near = Near;
	m_Far = Far;
	m_ViewAngle = ViewAngle;

	SetCameraType(eType);
}

void Camera_Com::SetWidth(float Width)
{
	m_Width = Width;
	SetCameraType(m_CameraType);
}

void Camera_Com::SetHeight(float Height)
{
	m_Height = Height;
	SetCameraType(m_CameraType);
}

void Camera_Com::SetViewAngle(float Angle)
{
	m_ViewAngle = Angle;
	SetCameraType(m_CameraType);
}

void Camera_Com::SetNear(float Near)
{
	m_Near = Near;
	SetCameraType(m_CameraType);
}

void Camera_Com::SetFar(float Far)
{
	m_Far = Far;
	SetCameraType(m_CameraType);
}

Matrix Camera_Com::GetViewMatrix() const
{
	return m_View;
}

Matrix Camera_Com::GetProjection() const
{
	return m_Projection;
}

void Camera_Com::SetTarget(GameObject* pTarget)
{
	m_Target = pTarget->GetTransform();
}

void Camera_Com::SetTarget(Component_Base* pTarget)
{
	m_Target = pTarget->GetTransform();
}