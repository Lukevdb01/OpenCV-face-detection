#include <Windows.h>
#include "utils/helper.h"
#include "components/window.h"
#include "components/gui_renderer.h"
#include <utils/config_loader.h>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Window* window = new Window();
	GuiRenderer* render = new GuiRenderer();
	Helper* helper = new Helper();
	ConfigLoader* config = new ConfigLoader("resources", "config.ini");

	if (!config)
	{
		MessageBox(NULL, L"Failed to load config file", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	cv::CascadeClassifier cascade;
	cascade.load(config->getString("opencv", "detection_method"));
	if (cascade.empty())
	{
		MessageBox(NULL, L"Failed to load cascade classifier", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	cv::Mat image;

	window->Initialize(L"OpenCV - Human Detection", config->GetInteger("application", "window_width"), config->GetInteger("application", "window_height"), hInstance, nShowCmd);
	render->Initialize(window->hwnd, window->hdc, window->hglrc);

	cv::VideoCapture cap(config->GetInteger("opencv", "capture_id"));
	if (!cap.isOpened())
	{
		MessageBox(NULL, L"Failed to open camera", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	cap.set(cv::CAP_PROP_FPS, config->GetInteger("application", "frames_per_second"));

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

			cap.read(image);

			std::vector<cv::Rect> detections;
			cascade.detectMultiScale(image, detections, 1.3, 5);

			for (int i = 0; i < detections.size(); i++)
			{
				cv::rectangle(image, detections[i].tl(), detections[i].br(), cv::Scalar(50, 50, 255), 3);
			}

			if (image.empty())
				MessageBox(NULL, L"Image is empty", L"Error", MB_OK | MB_ICONERROR);

			render->Render(image, helper, detections);

			SwapBuffers(window->hdc);
		}
	}

	return 0;
}