#include <Windows.h>
#include "utils/helper.h"
#include "components/window.h"
#include "components/gui_renderer.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Window* window = new Window();
	GuiRenderer* render = new GuiRenderer();
	Helper* helper = new Helper();

	window->Initialize(L"OpenCV - Human Detection", 800, 600, hInstance, nShowCmd);
	render->Initialize(window->hwnd, window->hdc, window->hglrc);

	cv::Mat image = cv::imread("picture.jpg");
	if (image.empty())
	{
		MessageBox(NULL, L"Failed to load image", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	cv::HOGDescriptor hog;
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

	std::vector<cv::Rect> detections;
	hog.detectMultiScale(image, detections, 0, cv::Size(8, 8), cv::Size(32, 32), 1.2, 2);

	for (auto& detection : detections)
	{
		cv::rectangle(image, detection.tl(), detection.br(), cv::Scalar(255, 0, 0), 2);
	}

	std::cout << "Number of people detected: " << detections.size() << std::endl;

	helper->ConvertMatToTexture(image);

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

			if (image.empty())
				MessageBox(NULL, L"Image is empty", L"Error", MB_OK | MB_ICONERROR);

			render->Render(image, helper);

			SwapBuffers(window->hdc);
		}
	}

	return 0;
}