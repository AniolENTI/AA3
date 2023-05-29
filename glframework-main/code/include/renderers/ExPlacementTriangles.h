#pragma once
#include "Renderer.h"
#include <Program.h>

class ExPlacementTriangles :
    public Renderer
{
    Program* program;
    GLuint VAO;
    GLuint VBO;

public:
    ExPlacementTriangles(int width, int height);
    ~ExPlacementTriangles();
protected:
    void render(float dt);
};