#pragma once

#include <vector>

#include "opencv2/core.hpp"

class Renderer
{
public:
    Renderer();

    void BindBuffers(std::vector<float> vertices, 
                     std::vector<unsigned int> indices);

    void Draw();
private:
    unsigned int m_vao, m_vbo, m_ebo;
};