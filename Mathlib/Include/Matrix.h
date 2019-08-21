#pragma once
//��� �����Ѵ�
#pragma warning(disable:4251)
#include "Vector4.h"

//����Ʈ���� ����Ʈ�� 16����� �����ش� (SIMD �������͸� ����Ѵ�)
union MATH_DLL __declspec(align(16)) Matrix
{
	XMMATRIX matrix;
	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};

	struct
	{
		Vector4 vec[4];
	};

	Matrix();
	Matrix(const Matrix& mat);
	Matrix(const XMMATRIX& mat);

	void operator = (const Matrix& mat);
	void operator = (const XMMATRIX& mat);

	Matrix operator * (const Matrix& mat) const;
	Matrix operator * (const XMMATRIX& mat)	const;
	Matrix operator * (float val) const;
	Matrix operator * (int val)	const;
	// *=
	Matrix& operator *= (const Matrix& mat);
	Matrix& operator *= (const XMMATRIX& mat);
	Matrix& operator *= (float val);
	Matrix& operator *= (int val);

	Vector4& operator[] (unsigned int Index);

	void* operator new (size_t Size);
	void operator delete (void* Data);

	//�׵����
	XMMATRIX Identity();
	//��ġ���
	XMMATRIX Transpose();
	//�����
	XMMATRIX Inverse();

	//S R T ���� ���ϱ�.
	XMMATRIX Scaling(float x, float y, float z);
	XMMATRIX Scaling(const union Vector3& Scale);
	XMMATRIX Rotation(float x, float y, float z);
	XMMATRIX Rotation(const union Vector3& Rot);
	XMMATRIX RotationX(float x);
	XMMATRIX RotationY(float y);
	XMMATRIX RotationZ(float z);

	//��ȸ��
	XMMATRIX RotationAxis(float Angle, union Vector3& Axis);

	//����̵�����.
	XMMATRIX Translation(float x, float y, float z);
	XMMATRIX Translation(const union Vector3& vPos);
};