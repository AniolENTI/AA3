#include <renderers/Renderer.h>

Renderer::Renderer(int width, int height)
{
	glViewport(0, 0, width, height);
	glClearColor(bgColor[0],bgColor[1],bgColor[2],bgColor[3]);
	glClearDepth(1.f);

	cam._projection = glm::perspective(FOV, (float)width / (float)height, zNear, zFar);

	glEnable(GL_DEPTH_TEST);

}

Renderer::~Renderer()
{

}

void Renderer::GUI()
{
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		/////////////////////////////////////////////////////TODO
		// Do your GUI code here....

		// Reset the camera position
		if (ImGui::Button("Reset Camera")) {
			panv[0] = panv[1] = panv[2] = 0.f;
			rota[0] = rota[1] = 0.f;
		}

		renderGUI();
		/////////////////////////////////////////////////////////
	}
	// .........................

	ImGui::End();

	// Example code -- ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	bool show_test_window = false;
	if (show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void Renderer::GLmousecb(MouseEvent ev)
{
	if (prevMouse.waspressed && prevMouse.button == ev.button) {
		float diffx = ev.posx - prevMouse.lastx;
		float diffy = ev.posy - prevMouse.lasty;
		switch (ev.button) {
		case MouseEvent::Button::Left: // ROTATE
			rota[0] += diffx * 0.005f;
			rota[1] += diffy * 0.005f;
			break;
		case MouseEvent::Button::Right: // MOVE XY
			panv[0] += diffx * 0.03f;
			panv[1] -= diffy * 0.03f;
			break;
		case MouseEvent::Button::Middle: // MOVE Z
			panv[2] += diffy * 0.05f;
			break;
		default: break;
		}
	}
	else {
		prevMouse.button = ev.button;
		prevMouse.waspressed = true;
	}
	prevMouse.lastx = ev.posx;
	prevMouse.lasty = ev.posy;
}

void Renderer::GLResize(int width, int height)
{
	glViewport(0, 0, width, height);
	if (height != 0) cam._projection = glm::perspective(FOV, (float)width / (float)height, zNear, zFar);
	else cam._projection = glm::perspective(FOV, 0.f, zNear, zFar);
}

void Renderer::GLrender(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam._modelView = glm::mat4(1.f);
	cam._modelView = glm::translate(cam._modelView, glm::vec3(panv[0], panv[1], panv[2]));
	cam._cameraRotationMat = glm::rotate(glm::mat4(), rota[1], glm::vec3(1.f, 0.f, 0.f));
	cam._cameraRotationMat = glm::rotate(cam._cameraRotationMat, rota[0], glm::vec3(0.f, 1.f, 0.f));

	cam._modelView *= cam._cameraRotationMat;

	cam._MVP = cam._projection * cam._modelView;

	/////////////////////////////////////////////////////TODO
	// Do your render code here
	render(dt);
	/////////////////////////////////////////////////////////

	ImGui::Render();
}

void Renderer::render(float dt)
{

}

void Renderer::renderGUI() {

}
