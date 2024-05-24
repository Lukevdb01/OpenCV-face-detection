#include <Windows.h>
#include "utils/opengl.h"
#include "components/window.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Window* window = new Window();

	window->Initialize(L"OpenCV - Human Detection", 800, 600, hInstance, nShowCmd);

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			SwapBuffers(window->hdc);
		}
	}

	return 0;
}