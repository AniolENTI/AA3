#pragma once
#include "Renderer.h"
#include <Program.h>

class ExTexture : public Renderer
{
public:
	ExTexture(int width, int height, glm::vec3 topRight, glm::vec3 topLeft, glm::vec3 botLeft, glm::vec3 botRight, char* path);
	~ExTexture();
	void render(float dt) override;
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	GLuint texture;

	Program* program;
};