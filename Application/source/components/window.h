#pragma once

#include <Windows.h>

class Window
{
public:
	Window() : hwnd(nullptr) 
	{
		
	}
	void Initialize(HINSTANCE hInstance, int nCmdShow);
	void Release();
public:
	HWND hwnd;
	HDC hdc = GetDC(hwnd);
	HGLRC hglrc = wglCreateContext(hdc);
	PIXELFORMATDESCRIPTOR pfd = {};
	static UINT m_ResizeWidth, m_ResizeHeight;
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

