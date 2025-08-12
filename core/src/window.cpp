#include "dxd/window.h"

#include "imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace DXD
{

	HINSTANCE UWindow::UWindowClass::GetInstance()
	{
		static UWindow::UWindowClass WindowClass;
		return WindowClass.HInstance;
	}

	const std::wstring& UWindow::UWindowClass::GetWindowClassName()
	{
		return WindowClassName;
	}

	UWindow::UWindowClass::~UWindowClass()
	{
		UnregisterClassW(GetWindowClassName().c_str(), GetInstance());
	}

	UWindow::UWindowClass::UWindowClass() : HInstance(GetModuleHandle(nullptr))
	{
		WNDCLASSEXW WndClass   = {};
		WndClass.cbSize		   = sizeof(WNDCLASSEXW);
		WndClass.style		   = CS_OWNDC;
		WndClass.lpfnWndProc   = WndProcSetup;
		WndClass.cbClsExtra	   = 0;
		WndClass.cbWndExtra	   = 0;
		WndClass.hInstance	   = HInstance;
		WndClass.hIcon		   = nullptr;
		WndClass.hCursor	   = nullptr;
		WndClass.hbrBackground = nullptr;
		WndClass.lpszMenuName  = nullptr;
		WndClass.lpszClassName = GetWindowClassName().c_str();
		WndClass.hIconSm	   = nullptr;

		RegisterClassExW(&WndClass);
	}

	const std::wstring UWindow::UWindowClass::WindowClassName = L"DirectX11 Demo";

	UWindow::~UWindow()
	{
		DestroyWindow(HWindow);
	}

	UWindow::UWindow(int Width, int Height, const std::wstring& WindowName)
		: Width(Width), Height(Height)
	{
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
		if (ImGui_ImplWin32_WndProcHandler(hWindow, message, wParam, lParam))
		{
			return true;
		}

		switch (message)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_KILLFOCUS:
			Keyboard.ClearState();
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000) || Keyboard.IsAutoRepeatEnabled())
			{
				Keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			Keyboard.OnChar(static_cast<unsigned char>(wParam));
			break;
		case WM_MOUSEMOVE:
		{
			const POINTS Points = MAKEPOINTS(lParam);
			if (0 <= Points.x && Points.x < Width && 0 <= Points.y && Points.y < Height)
			{
				Mouse.OnMouseMove(Points.x, Points.y);
				if (!Mouse.IsInsideWindow())
				{
					SetCapture(HWindow);
					Mouse.OnMouseEnter();
				}
			}
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					Mouse.OnMouseMove(Points.x, Points.y);
				}
				else
				{
					ReleaseCapture();
					Mouse.OnMouseLeave();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			SetForegroundWindow(HWindow);
			const POINTS Points = MAKEPOINTS(lParam);
			Mouse.OnLeftPressed(Points.x, Points.y);
			break;	
		}
		case WM_LBUTTONUP:
		{
			const POINTS Points = MAKEPOINTS(lParam);
			Mouse.OnLeftReleased(Points.x, Points.y);
			if (!(0 <= Points.x && Points.x < Width && 0 <= Points.y && Points.y < Height))
			{
				ReleaseCapture();
				Mouse.OnMouseLeave();
			}		
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS Points = MAKEPOINTS(lParam);
			Mouse.OnRightPressed(Points.x, Points.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS Points = MAKEPOINTS(lParam);
			Mouse.OnRightReleased(Points.x, Points.y);
			if (!(0 <= Points.x && Points.x < Width && 0 <= Points.y && Points.y < Height))
			{
				ReleaseCapture();
				Mouse.OnMouseLeave();
			}		
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS Points = MAKEPOINTS(lParam);
			const int Delta = GET_WHEEL_DELTA_WPARAM(wParam);
			Mouse.OnWheelDelta(Points.x, Points.y, Delta);
			break;
		}
		}

		return DefWindowProc(hWindow, message, wParam, lParam);
	}

} // namespace DXD