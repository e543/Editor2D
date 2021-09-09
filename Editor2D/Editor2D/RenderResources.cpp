#include "RenderResources.h"


RenderResources::RenderResources(HWND hwnd) : hwnd(hwnd)
{
	if (OnCreate())
		CreateGraphicsResources();
	else
		RenderResources::~RenderResources();
}
RenderResources::~RenderResources()
{
	DiscardGraphicsResources();
}

bool RenderResources::OnCreate()
{
	if (FAILED(D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
	{
		return false;
	}
	return true;
}

HRESULT RenderResources::CreateGraphicsResources()
{
	HRESULT hr = S_OK;
	if (pRenderTarget == nullptr)
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, size),
			&pRenderTarget);
		if (SUCCEEDED(hr))
		{
			hr = CreateAdditionalGraphicsResources();
		}
	}
	return hr;
}

void RenderResources::Resize()
{
	if (pRenderTarget != nullptr)
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		pRenderTarget->Resize(size);
		Render(context);
		InvalidateRect(hwnd, nullptr, FALSE);
	}
}

HRESULT RenderResources::CreateAdditionalGraphicsResources()
{
	HRESULT hr = S_OK;

	hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0), &pBrush);;
	return hr;
}

void RenderResources::DiscardGraphicsResources()
{
	SAFE_RELEASE(pFactory);
	SAFE_RELEASE(pRenderTarget);
	SAFE_RELEASE(pBrush);
}


void RenderResources::Render(GeometryController::RenderContext& context)
{
	this->context = context;
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		pRenderTarget->BeginDraw();
		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		RenderThunk();
		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(hwnd, &ps);
	}
}

void RenderResources::RenderThunk()
{
	// Render lines
	for (auto riter = context.lines.rbegin(); riter != context.lines.rend(); ++riter)
		(*riter)->Draw(pRenderTarget, pBrush);

	// Render splines
	for (auto riter = context.splines.rbegin(); riter != context.splines.rend(); ++riter)
		(*riter)->Draw(pRenderTarget, pBrush);

	// Render points
	for (auto riter = context.points.rbegin(); riter != context.points.rend(); ++riter)
		(*riter)->Draw(pRenderTarget, pBrush);

}