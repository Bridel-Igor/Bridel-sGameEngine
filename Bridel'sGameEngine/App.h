#pragma once
#include "Window.h"
#include "Timer.h"

class App
{
public:
	App();
	// master frame / message loop
	int go();
private:
	void doFrame();
private:
	Window wnd;
	Timer timer;

	int wheelRoll = 0, x = 0, y = 0;//
	bool mouseInClient = false;//
	std::string word;//
};