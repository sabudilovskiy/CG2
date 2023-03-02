#pragma once
#include "Matrix.h"


Matrix<float> Translation(float x, float y);
Matrix<float> Identity();
Matrix<float> Rotation(float c, float s);
Matrix<float> Rotation(float rad);
Matrix<float> Scaling(float kx, float ky);
Matrix<float> ReflectOX();
Matrix<float> ReflectOY();
Matrix<float> ReflectAll();


