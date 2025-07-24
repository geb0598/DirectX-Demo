#include "dxd/window.h"

namespace dxd
{

	HINSTANCE UWindow::UWindowClass::GetInstance()
	{
		static UWindow::UWindowClass WindowClass;
		return WindowClass.hInstance;
	}

	const std::wstring& UWindow::UWindowClass::GetWindowClassName()
	{
		return WindowClassName;
	}

	UWindow::UWindowClass::~UWindowClass()
	{
		UnregisterClassW(GetWindowClassName().c_str(), GetInstance());
	}

	UWindow::UWindowClass::UWindowClass() : hInstance(GetModuleHandle(nullptr))
	{
		// TODO: EX or None EX
		// TODO: EX or EXW
		WNDCLASSEXW WndClass = {};
		WndClass.cbSize = sizeof(WNDCLASSEXW);
		WndClass.style = CS_OWNDC;
		WndClass.lpfnWndProc = WndProcSetup;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hInstance = hInstance;
		WndClass.hIcon = nullptr;
		WndClass.hCursor = nullptr;
		WndClass.hbrBackground = nullptr;
		WndClass.lpszMenuName = nullptr;
		WndClass.lpszClassName = GetWindowClassName().c_str();
		WndClass.hIconSm = nullptr;

		RegisterClassExW(&WndClass);
	}

	const std::wstring UWindow::UWindowClass::WindowClassName = L"DirectX11 Demo";

	UWindow::~UWindow()
	{
		DestroyWindow(HWindow);
	}

	UWindow::UWindow(int Width, int Height, const std::wstring& WindowName)
	{
		// TODO: EX or None EX
		// TODO: EX or EXW (Macro implictily expands to W version)
		HWindow = CreateWindowW(
			UWindowClass::GetWindowClassName().c_str(), WindowName.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, Width, Height,
			nullptr, nullptr, UWindowClass::GetInstance(), this
		);

		ShowWindow(HWindow, SW_SHOWDEFAULT);
	}

	HWND UWindow::GetHWindow()
	{
		return HWindow;
	}

	LRESULT CALLBACK UWindow::WndProcSetup(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_NCCREATE)
		{
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lParam);
			UWindow* const window = static_cast<UWindow*>(create->lpCreateParams);
			SetWindowLongPtr(hWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			SetWindowLongPtr(hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&UWindow::WndProc));
			return window->WndProcImpl(hWindow, message, wParam, lParam);
		}

		return DefWindowProc(hWindow, message, wParam, lParam);
	}


	LRESULT CALLBACK UWindow::WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UWindow* const window = reinterpret_cast<UWindow*>(GetWindowLongPtr(hWindow, GWLP_USERDATA));
		return window->WndProcImpl(hWindow, message, wParam, lParam);
	}

	LRESULT UWindow::WndProcImpl(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWindow, message, wParam, lParam);
	}

} // namespace dxd