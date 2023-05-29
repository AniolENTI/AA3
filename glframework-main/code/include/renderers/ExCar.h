#pragma once
#include "Renderer.h"
#include <Program.h>

class ExCar : public Renderer
{
public:
	ExCar(int width, int height);
	~ExCar();
	void render(float dt);
private:
};