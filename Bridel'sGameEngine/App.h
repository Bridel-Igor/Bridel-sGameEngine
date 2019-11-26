#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Mesh.h"
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
	void showImguiDemoWindow();
private:
	bool showDemoWindow = false;
	ImGuiManager imGui;
	Window wnd;
	Timer timer;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight light;
	Model nano{ wnd.gfx(), "Models\\nano.gltf" };
};