
#include "Scene2D.h"


void Scene2D::draw(const HDC& hdc, HWND hWnd)
{
	for (auto& object : m_objects)
		object._Draw(hdc, hWnd);
			
}

void Scene2D::addObject(Model2D model)
{
	m_objects.emplace_back(std::move(model));
}
