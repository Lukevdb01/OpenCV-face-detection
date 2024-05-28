#include <Windows.h>
#include "utils/helper.h"
#include "components/window.h"
#include "components/gui_renderer.h"
#include "utils/config_loader.h"
#include "utils/serial.h"
#include <future>
#include <opencv2/opencv.hpp>
#include <mutex>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    Window* window = new Window();
    GuiRenderer* render = new GuiRenderer();
    Helper* helper = new Helper();
    ConfigLoader* config = new ConfigLoader("resources", "config.ini");
    Serial* serial = new Serial();

    if (!config || !serial->Open(config->getString("serial", "com_port"), config->GetInteger("serial", "baud_rate"))) {
        MessageBox(NULL, L"Failed to load config file or open serial port", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    cv::CascadeClassifier cascade;
    if (!cascade.load(config->getString("opencv", "detection_method")) || cascade.empty()) {
        MessageBox(NULL, L"Failed to load cascade classifier", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    window->Initialize(L"OpenCV - Human Detection", config->GetInteger("application", "window_width"), config->GetInteger("application", "window_height"), hInstance, nShowCmd);
    render->Initialize(window->hwnd, window->hdc, window->hglrc);

    cv::VideoCapture cap(config->GetInteger("opencv", "capture_id"));
    if (!cap.isOpened()) {
        MessageBox(NULL, L"Failed to open camera", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    cap.set(cv::CAP_PROP_FPS, config->GetInteger("application", "frames_per_second"));

    MSG msg = {};
    int previousState = -1;
    std::vector<cv::Rect> detections;
    std::mutex detectionMutex;

    auto DetectAndWriteAsync = [&]() {
        while (msg.message != WM_QUIT) {
            int currentState;
            {
                std::lock_guard<std::mutex> lock(detectionMutex);
                switch (detections.size()) {
                case 0: currentState = 1; break;
                case 2: currentState = 3; break;
                case 4: currentState = 5; break;
                default: currentState = 0; break;
                }
            }

            if (currentState != previousState) {
                Sleep(config->GetInteger("serial", "async_delay"));
                serial->Write(currentState);
                previousState = currentState;
            }
        }
        };

    std::future<void> detectionFuture = std::async(std::launch::async, DetectAndWriteAsync);

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

            cv::Mat image;
            if (!cap.read(image) || image.empty()) {
                MessageBox(NULL, L"Failed to read image from camera", L"Error", MB_OK | MB_ICONERROR);
                continue;
            }

            {
                std::lock_guard<std::mutex> lock(detectionMutex);
                cascade.detectMultiScale(image, detections, 1.3, 5);
            }

            for (const auto& rect : detections) {
                cv::rectangle(image, rect.tl(), rect.br(), cv::Scalar(50, 50, 255), 3);
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
