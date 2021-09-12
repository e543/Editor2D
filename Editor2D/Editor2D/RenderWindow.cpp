#include "RenderWindow.h"



bool RenderWindow::OnCreate()
{
	pRes = std::make_unique<RenderResources>(m_hwnd);
	if (!pRes)
		return false;
	else
		return true;
}

std::optional<int> RenderWindow::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{

			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

RenderResources& RenderWindow::GetRes()
{
	if (!pRes)
	{
		MessageBox(nullptr, L"RenderResources nullptr", L"NoRes Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return *pRes;
}



LRESULT RenderWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (!RenderWindow::OnCreate())
		{
			return S_FALSE;
		}
		return S_OK;

	case WM_SIZE:
		pRes->Resize();
		return S_OK;
	case WM_DESTROY:
		PostQuitMessage(0);
		return S_OK;

	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMouseMove(pt.x, pt.y);
		return S_OK;
	}
	case WM_LBUTTONDOWN:
	{
		mouse.OnLeftPressed();
		SetCapture(m_hwnd);
		return S_OK;
	}
	case WM_LBUTTONUP:
	{
		mouse.OnLeftReleased();
		ReleaseCapture();
		return S_OK;
	}
	case WM_RBUTTONDOWN:
	{
		mouse.OnRightPressed();
		SetCapture(m_hwnd);
		return S_OK;
	}
	case WM_RBUTTONUP:
	{
		mouse.OnRightReleased();
		ReleaseCapture();
		return S_OK;
	}
	default:;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}



