#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Model.h"
#include <set>

class App
{
public:
	App();
	// master frame / message loop
	int go();
	~App();
private:
	void doFrame();
	void showModelWindow();
private:
	ImGuiManager imGui;
	Window wnd;
	Timer timer;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight light;
	Model nano{ wnd.gfx(), "Models\\nanosuit.obj" };
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};