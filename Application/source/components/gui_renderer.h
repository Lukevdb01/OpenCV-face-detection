#pragma once

#include <Windows.h>

#include "utils/helper.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

class GuiRenderer
{
public:
	GuiRenderer() {
		helper = new Helper();
	}

	void Initialize(HWND hwnd, HDC hdc, HGLRC hglrc);
	void Render(cv::Mat image, Helper* helper);
	void Shutdown();
private:
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	Helper* helper = nullptr;
};

