#include "Loader.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

Loader::Loader(std::string path)
{
    m_load = Load(std::move(path));
}

std::vector<Vector2D>&& Loader::TakeVerticles()
{
    return std::move(m_vertices);
}

Loader::Loader(): m_load(false){}

std::vector<int>&& Loader::TakeIndices()
{
    return std::move(m_indices);
}

bool Loader::Load(std::string path)
{
    std::vector<float>vertices1;
    std::vector<float>indices1;
    bool flag = true;
    std::string line1;
    int i = 0;
    std::ifstream input(std::move(path));
    if (!input.is_open())
        return false;
    std::vector<int> order;
    std::string line;
    while (std::getline(input, line))
    {

        for (int i = 2; i < line.size(); i++)
            line1.push_back(line[i]);

        std::istringstream parse(line1);
        float n;
        if (line[0] == 'v')
        {
            while (parse >> n)
            {

                if (flag)
                {

                    vertices1.push_back(n);
                    flag = false;
                }
                else
                {
                    indices1.push_back(n);
                    flag = true;
                }
            }
            line1.clear();
        }
        else
        {
            while (parse >> n)
                m_indices.push_back(n);
        }

    }
    input.close();
    for (int i = 0; i < vertices1.size(); i++)
    {
        Vector2D dot(vertices1[m_indices[i] - 1], indices1[m_indices[i] - 1]);

        m_vertices.push_back(dot);
    }

    return true;
}

bool Loader::isLoad()
{
    return m_load;
}

