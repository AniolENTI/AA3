#pragma once
#include "Renderer.h"
#include <Program.h>

#include "ExTexture.h"

class ExCar : public Renderer
{
public:
	ExCar(int width, int height);
	~ExCar();
	void render(float dt);
private:
	Program* program;
	GLuint VAO;
	GLuint VBO;



	ExTexture* texture;
	glm::vec3 topRight = { 0.5f, 0.5f, 0 };
	glm::vec3 topLeft = { -0.5f, 0.5f, 0 };
	glm::vec3 botLeft = { -0.5f, -0.5f, 0 };
	glm::vec3 botRight = { 0.5f, -0.5f, 0 };
	char* path = "textures/gotele.jpg";
};