#pragma once
#include "Renderer.h"
#include <Program.h>
#include <GL/glew.h>
#include <glm\gtc\type_ptr.hpp>

class ExTexture
{
public:
	ExTexture(glm::vec3 topRight, glm::vec3 topLeft, glm::vec3 botLeft, glm::vec3 botRight, char* path);
	~ExTexture();
	void setTransforms(CameraTransforms cam);
	void draw();
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	GLuint texture;

	Program* program;

	CameraTransforms cam;
};