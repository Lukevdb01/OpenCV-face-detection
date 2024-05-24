#pragma once

#include <Windows.h>

class Window
{
public:
	Window() : hwnd(nullptr) {}
	void Initialize(LPCWSTR TITLE, int WIDTH, int HEIGHT, HINSTANCE hInstance, int nCmdShow);
	void Release();
public:
	HWND hwnd = {};
	HDC hdc = {};
	HGLRC hglrc = {};
	PIXELFORMATDESCRIPTOR pfd = {};
	static UINT m_ResizeWidth, m_ResizeHeight;
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

