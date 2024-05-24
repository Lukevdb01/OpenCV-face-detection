#include "window.h"
#include "utils/opengl.h"

UINT Window::m_ResizeWidth = 0;
UINT Window::m_ResizeHeight = 0;

void Window::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	// Create window class reference
	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"OpenGLOpenCVDetection", NULL };
	RegisterClassExW(&wc);

	// Create window
	hwnd = CreateWindow(wc.lpszClassName, L"OpenCV - Human Detection", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL);

	// Check if HDC is != null
	if (!hdc)
		hdc = GetDC(hwnd);

	// Set pixel format
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int format = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, format, &pfd);

	// Create OpenGL context
	if (!hglrc)
		hglrc = wglCreateContext(hdc);

	// Make OpenGL context current
	wglMakeCurrent(hdc, hglrc);

	// Show window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
}

void Window::Release()
{
	DestroyWindow(hwnd);
	delete hdc;
	delete hglrc;

	UnregisterClass(L"OpenGLOpenCVDetection", GetModuleHandle(NULL));
}

LRESULT Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		Window::m_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		Window::m_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hwnd, message, wParam, lParam);
}
