#pragma once
#include <vector>
#include "Vector2D.h"
#include "Matrix.h"
#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include "AffineTransformation.h"
class Model2D
{
public:
	void _Draw(const HDC& hdc, HWND hWnd);

	void setScale(const float& scale)
	{
		Matrix m(3, 1);
		for (int i = 0; i < m_vertices.size(); i++)
		{

			auto& [x, y] = m_vertices[i];
			m.setValue(0, 0, x);
			m.setValue(1, 0, y);
			m.setValue(2, 0, 1);
			auto M = Scaling(scale, scale) * m;
			
			x = M.ReturnValue(0, 0);
			y = M.ReturnValue(1, 0);

		}
	}

	void setRotation(const float& angle)
	{

		Matrix m(3, 1);
		for (int i = 0; i < m_vertices.size(); i++)
		{
			auto& [x, y] = m_vertices[i];
			m.setValue(0, 0, x);
			m.setValue(1, 0, y);
			m.setValue(2, 0, 1);
			Matrix<> M = Rotation(angle) * m;
			x =  M.ReturnValue(0, 0);
			y =  M.ReturnValue(1, 0);

		}


	}
	void setPosition(const Vector2D& position)
	{
		Matrix m(3, 1);
		for (int i = 0; i < m_vertices.size(); i++)
		{
			auto& [x, y] = m_vertices[i];
			m.setValue(0, 0, x);
			m.setValue(1, 0, y);
			m.setValue(2, 0, 1);
			Matrix<> M = Translation(position.x, position.y) * m;
			x = M.ReturnValue(0, 0);
			y = M.ReturnValue(1, 0);
		}
	}

	void Mirror(int k)
	{
		if (k == 0)
		{
			Matrix m(3, 1);
			for (int i = 0; i < m_vertices.size(); i++)
			{
				auto& [x, y] = m_vertices[i];
				m.setValue(0, 0, x);
				m.setValue(1, 0, y);
				m.setValue(2, 0, 1);
				Matrix<> M = ReflectOX() * m;
				x = M.ReturnValue(0, 0);
				y = M.ReturnValue(1, 0);

			}
		}

		if (k == 1)

		{
			Matrix m(3, 1);
			for (int i = 0; i < m_vertices.size(); i++)
			{
				auto& [x, y] = m_vertices[i];
				m.setValue(0, 0, x);
				m.setValue(1, 0, y);
				m.setValue(2, 0, 1);
				Matrix M = ReflectOY() * m;
				x = M.ReturnValue(0, 0);
				y = M.ReturnValue(1, 0);
			}
		}

	}


	enum class Axis { x = 0, y };

	Model2D(std::vector<Vector2D> vertices, std::vector<int> indices);


	std::vector<Vector2D> m_vertices;
	std::vector<int> m_indices;

	Matrix<> m_modelMatrix = Identity();
};

