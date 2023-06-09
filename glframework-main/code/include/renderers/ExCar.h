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
	//COTXE
	Program* program;
	GLuint VAO;
	GLuint VBO;


	//PLA
	ExTexture* texture;
	glm::vec3 topRight	= {  50.0f, -10.75f, -50.0f };
	glm::vec3 topLeft	= { -50.0f, -10.75f, -50.0f };
	glm::vec3 botLeft	= { -50.0f, -10.75f,  50.0f };
	glm::vec3 botRight	= {	 50.0f, -10.75f,  50.0f };
	char* path = "textures/gotele.jpg";

	//MIRALL
	ExTexture* mirror;
	GLuint fbo, fbo_tex;
	void setupFBO();

	glm::vec3 topRightMirror = { 0.5f, 0.5f, 0 };
	glm::vec3 topLeftMirror = { -0.5f, 0.5f, 0 };
	glm::vec3 botLeftMirror = { -0.5f, -0.5f, 0 };
	glm::vec3 botRightMirror = { 0.5f, -0.5f, 0 };

	int width, height;
};