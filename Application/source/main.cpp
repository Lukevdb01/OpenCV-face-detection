#include <Windows.h>
#include "utils/helper.h"
#include "components/window.h"
#include "components/gui_renderer.h"
#include "utils/config_loader.h"
#include "utils/serial.h"
#include <future>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Window* window = new Window();
	GuiRenderer* render = new GuiRenderer();
	Helper* helper = new Helper();
	ConfigLoader* config = new ConfigLoader("resources", "config.ini");
	Serial* serial = new Serial();

	if (!config)
	{
		MessageBox(NULL, L"Failed to load config file", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	serial->Open(config->getString("serial", "com_port"), config->GetInteger("serial", "baud_rate"));

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


    int previousState = -1; // Variable to store the previous state


    auto DetectAndWriteAsync = [&]() {
        int previousState = -1; // Variable to store the previous state
        while (msg.message != WM_QUIT) {
            cap.read(image);

            std::vector<cv::Rect> detections;
            cascade.detectMultiScale(image, detections, config->GetFloat("opencv", "scale_factor"), config->GetInteger("opencv", "neightbors"), 0, cv::Size(30, 30), cv::Size());

            int currentState;
            if (detections.size() == 0) {
                currentState = 1;
            }
            else if (detections.size() == 2) {
                currentState = 3;
            }
            else if (detections.size() == 4) {
                currentState = 5;
            }
            else {
                currentState = 0;
            }

            if (currentState != previousState) {
				Sleep(config->GetInteger("serial", "async_delay")); // Sleep for 1 second
                serial->Write(currentState);
                previousState = currentState;
            }
        }
    };

    std::future<void> detectionFuture = std::async(std::launch::async, DetectAndWriteAsync);

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

            if (image.empty()) {
                MessageBox(NULL, L"Image is empty", L"Error", MB_OK | MB_ICONERROR);
            }

            render->Render(image, helper, detections);

            SwapBuffers(window->hdc);
        }
    }

	detectionFuture.wait();

	render->Shutdown();
	delete window;
	delete render;
	delete helper;
	delete config;
	delete serial;

	return 0;
}