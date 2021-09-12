#pragma once
#include "RenderWindow.h"
#include "GeometryController.h"
#include "RenderWindow.h"


class Application
{
	GeometryController gc;
	RenderWindow rwnd;
	void HandleInput();
	void DoFrame();
public:
	Application();
	int Go();
};


