#include "Model2D.h"
#include <windows.h>
#include <windef.h>
#include <wingdi.h>
#include <windowsx.h>
#include <gdiplus.h>
#include <Windows.h>
#include "AffineTransformation.h"

enum class Axis { x = 0, y };


Model2D::Model2D(std::vector<Vector2D> vertices, std::vector<int> indices) :
	m_vertices(std::move(vertices)),
	m_indices(std::move(indices))
{
}
float GetWindowCordX(float x, HWND hWnd)
{
	RECT rect;

	GetWindowRect(hWnd, &rect);
	x = x + (rect.right - rect.left) / 2;
	return x;
}
float GetWindowY(float y, HWND hWnd)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);

	y = y + (rect.bottom - rect.top) / 2;
	return y;
}



void Model2D::_Draw(const HDC& hdc, HWND hWnd)
{
	MoveToEx(hdc, GetWindowCordX(m_vertices[m_vertices.size() - 1].x, hWnd), GetWindowY(m_vertices[m_vertices.size() - 1].y, hWnd), NULL);
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		LineTo(hdc, GetWindowCordX(m_vertices[i].x, hWnd), GetWindowY(m_vertices[i].y, hWnd));
	}
}
