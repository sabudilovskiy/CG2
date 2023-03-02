#include "AffineTransformation.h"

Matrix<float> Translation(float x, float y)
{

	int r = 3, c = 3;

	float m[9] = { 1, 0, x ,
					0, 1, y ,
					0, 0, 1 };


	Matrix<float> A(r, c, m);



	return A;

}

Matrix<float> Identity()
{
	int r = 3, c = 3;

	float m[9] = { 1, 0, 0,
					0, 1, 0,
					0, 0, 1 };
	Matrix<float> A(r, c, m);
	return A;
}

Matrix<float> Rotation(float c, float s)
{
	float hypotenuse = sqrt(c * c + s * s);

	c = c / hypotenuse;
	s = s / hypotenuse;
	int r = 3, _c = 3;

	float m[9] = { c, s, 0 ,
					-s, c, 0 ,
					0, 0, 1 };
	Matrix<float> A(r, _c, m);
	return A;

}

Matrix<float> Rotation(float rad)
{
	return Rotation(cos(rad), sin(rad));
}

Matrix<float> Scaling(float kx, float ky)
{
	int r = 3, _c = 3;
	float m[9] = { kx, 0, 0 ,
					0, ky, 0 ,
					0, 0, 1 };
	Matrix<float> A(r, _c, m);
	return A;

}

Matrix<float> ReflectOX()
{
	return Scaling(-1, 1);
}

Matrix<float> ReflectOY()
{
	return Scaling(1, -1);
}

Matrix<float> ReflectAll()
{
	return Scaling(-1, -1);
}


