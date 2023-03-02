#pragma once
#include <string>
#include <vector>
#include "Vector2D.h"

class Loader
{	std::vector<Vector2D> m_vertices;
	std::vector<int> m_indices;
	bool m_load;
	bool Load(std::string path);
public:
	Loader();
	Loader(std::string path);
	std::vector<Vector2D>&& TakeVerticles();
	std::vector<int>&& TakeIndices();
	bool isLoad();
};

