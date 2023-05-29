#pragma once
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>

#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>

#include "GL_framework.h"

struct CameraTransforms {
	glm::mat4 _projection;
	glm::mat4 _modelView;
	glm::mat4 _MVP;
	glm::mat4 _cameraRotationMat;
	glm::mat4 _inv_modelview;
	glm::vec4 _cameraPoint;
};

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();
	void GUI();
	void GLmousecb(MouseEvent ev);
	void GLResize(int width, int height);
	void GLrender(float dt);
protected:
	float FOV = glm::radians(65.f);
	float zNear = 1.f;
	float zFar = 50.f;

	CameraTransforms cam;

	struct prevMouse {
		float lastx, lasty;
		MouseEvent::Button button = MouseEvent::Button::None;
		bool waspressed = false;
	} prevMouse;

	float panv[3] = { 0.f, 0.f, 0.f };
	float rota[2] = { 0.f, 0.f };

	const GLfloat bgColor[4] = { 0.2f,0.2f,0.2f,1.0f };

	virtual void render(float dt);
	virtual void renderGUI();
};

