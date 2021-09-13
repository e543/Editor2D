#pragma once
#include <windows.h>
#include "resource.h"


template <class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = NULL;
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	BaseWindow() : m_hwnd(NULL) { }
	virtual BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	)
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.hIcon = static_cast<HICON>(LoadImage(
			GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1),
			IMAGE_ICON, 32, 32, 0
		));
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = ClassName();
		wc.hIconSm = static_cast<HICON>(LoadImage(
			GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1),
			IMAGE_ICON, 16, 16, 0
		));
		RegisterClassEx(&wc);


		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
		);

		if (!m_hwnd)
			return FALSE;

		return (m_hwnd ? TRUE : FALSE);
	}
	HWND Window() const { return m_hwnd; }
protected:
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	virtual PCWSTR  ClassName() const = 0;
	HWND m_hwnd;
};



