#pragma once
#include <GL/glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <Program.h>

class Axis
{
public:
	Axis();
	~Axis();
	void setTransforms(glm::mat4 MVP);
	void draw();
	void toggle();
private:
	GLuint VAO;
	GLuint VBO[3];
	Program* program;

	glm::mat4 MVP;

	bool isActive = false;
};
