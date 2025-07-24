#pragma once

#include <string>

#include <Windows.h>

namespace dxd
{

	class UWindow
	{
	public:
		~UWindow();

		UWindow(int Width, int Height, const std::wstring& WindowName);

		UWindow(const UWindow&) = delete;
		UWindow(UWindow&&) noexcept = default;

		UWindow& operator=(const UWindow&) = delete;
		UWindow& operator=(UWindow&&) noexcept = default;

		HWND GetHWindow();

	private:
		class UWindowClass
		{
		public:
			static HINSTANCE GetInstance();
			static const std::wstring& GetWindowClassName();

			~UWindowClass();

			UWindowClass(const UWindowClass&) = delete;
			UWindowClass(UWindowClass&&) = delete;

			UWindowClass& operator=(const UWindowClass&) = delete;
			UWindowClass& operator=(UWindowClass&&) = delete;

		private:
			// TODO: Should I use std::string for coherent usage or std::wstring for compatibility?
			static const std::wstring WindowClassName;

			UWindowClass();

			HINSTANCE hInstance;
		};

		static LRESULT CALLBACK WndProcSetup(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT WndProcImpl(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam); // TODO: CALLBACK specifier required?

		HWND HWindow;
	};

} // namespace dxd
