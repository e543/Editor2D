#pragma once
#include <optional>
#include <memory>

#include "basewin.h"
#include "RenderResources.h"
#include "Keyboard.h"
#include "Mouse.h"



class RenderWindow : public BaseWindow<RenderWindow>
{
	std::unique_ptr<RenderResources> pRes;
	PCWSTR ClassName() const override { return L"Render window"; }
	bool OnCreate();
public:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	static std::optional<int> ProcessMessages();
	RenderResources& GetRes();

	Mouse mouse;
	Keyboard kbd;
};

