#pragma once
#include <memory>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }
#include "GeometryController.h"

class RenderResources
{
	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1SolidColorBrush* pBrush;

	HWND hwnd;
	GeometryController::RenderContext context;


	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	HRESULT CreateAdditionalGraphicsResources();
	bool OnCreate();

	void RenderThunk();
public:
	RenderResources(HWND hwnd);
	~RenderResources();
	void Resize();
	void Render(GeometryController::RenderContext& context);
};

