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
		WNDCLASSEXW wndclass = {};
		wndclass.cbSize = sizeof(WNDCLASSEXW);
		wndclass.style = CS_OWNDC;
		wndclass.lpfnWndProc = WndProcSetup;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = nullptr;
		wndclass.hCursor = nullptr;
		wndclass.hbrBackground = nullptr;
		wndclass.lpszMenuName = nullptr;
		wndclass.lpszClassName = GetWindowClassName().c_str();
		wndclass.hIconSm = nullptr;

		RegisterClassExW(&wndclass);
	}

	const std::wstring UWindow::UWindowClass::WindowClassName = L"DirectX11 Demo";

	UWindow::~UWindow()
	{
		DestroyWindow(hWnd);
	}

	UWindow::UWindow(int width, int height, const std::wstring& name)
	{
		// TODO: EX or None EX
		// TODO: EX or EXW (Macro implictily expands to W version)
		hWnd = CreateWindowW(
			UWindowClass::GetWindowClassName().c_str(), name.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			nullptr, nullptr, UWindowClass::GetInstance(), this
		);

		ShowWindow(hWnd, SW_SHOWDEFAULT);
	}

	HWND UWindow::GetHWND()
	{
		return hWnd;
	}

	LRESULT CALLBACK UWindow::WndProcSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_NCCREATE)
		{
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lParam);
			UWindow* const window = static_cast<UWindow*>(create->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&UWindow::WndProc));
			return window->WndProcImpl(hWnd, message, wParam, lParam);
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}


	LRESULT CALLBACK UWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UWindow* const window = reinterpret_cast<UWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return window->WndProcImpl(hWnd, message, wParam, lParam);
	}

	LRESULT UWindow::WndProcImpl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

} // namespace dxd