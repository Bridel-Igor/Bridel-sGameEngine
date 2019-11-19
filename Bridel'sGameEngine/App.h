#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"

class App
{
public:
	App();
	// master frame / message loop
	int go();
	~App();
private:
	void doFrame();
private:
	ImGuiManager imGui;
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	float speed_factor = 1.0f;
	static constexpr size_t nDrawables = 180;
};