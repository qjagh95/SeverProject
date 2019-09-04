#include "MathHeader.h"

float MATH_DLL RadianToDegree(float radian)
{
	return radian / JEONG_PI * 180.f;
}

float MATH_DLL DegreeToRadian(float degree)
{
	return degree / 180.f * JEONG_PI;
}