#pragma once
#include "Model2D.h"
#include <wtypes.h>

class Scene2D
{
public:
	void draw(const HDC& hdc, HWND hWnd);
	void addObject(Model2D model);

	auto& get_object(int i)
	{
		return  m_objects[i];
	}
	int size()
	{
		return m_objects.size();
	}
private:
	std::vector<Model2D> m_objects;
};

