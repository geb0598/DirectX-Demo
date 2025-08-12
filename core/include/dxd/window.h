#pragma once

#include <string>

#include <Windows.h>

#include "dxd/keyboard.h"
#include "dxd/mouse.h"

namespace DXD
{

	class UWindow
	{
	public:
		~UWindow();

		UWindow(int Width, int Height, const std::wstring& WindowName);

		UWindow(const UWindow&)			   = delete;
		UWindow& operator=(const UWindow&) = delete;

		UWindow(UWindow&&) noexcept			   = default;
		UWindow& operator=(UWindow&&) noexcept = default;

		HWND GetHWindow();

		UKeyboard Keyboard;
		UMouse Mouse;

	private:
		class UWindowClass
		{
		public:
			static HINSTANCE GetInstance();
			static const std::wstring& GetWindowClassName();

			~UWindowClass();

			UWindowClass(const UWindowClass&)			 = delete;
			UWindowClass& operator=(const UWindowClass&) = delete;

			UWindowClass(UWindowClass&&)			= delete;
			UWindowClass& operator=(UWindowClass&&) = delete;

		private:
			static const std::wstring WindowClassName;

			UWindowClass();

			HINSTANCE HInstance;
		};

		static LRESULT CALLBACK WndProcSetup(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam);
		 // TODO: CALLBACK specifier required?
		LRESULT WndProcImpl(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam);

		HWND HWindow;
		int Width;
		int Height;
	};

} // namespace DXD
