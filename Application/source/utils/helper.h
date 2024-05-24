#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "imgui/imgui.h"

#include <opencv2/opencv.hpp>

// Function pointer typedef for glGenerateMipmap
typedef void (APIENTRY* PFNGLGENERATEMIPMAPPROC)(GLenum target);
// Declare function pointer variable
static PFNGLGENERATEMIPMAPPROC glGenerateMipmap = reinterpret_cast<PFNGLGENERATEMIPMAPPROC>(wglGetProcAddress("glGenerateMipmap"));

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Helper {
public:
	GLuint textureID;

	cv::Mat ConvertMatToTexture(cv::Mat image) {
		if (image.empty()) {
			MessageBox(NULL, L"Image is empty", L"Error", MB_OK | MB_ICONERROR);
		}

        cv::cvtColor(image, image, cv::COLOR_BGR2RGBA);
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.ptr());
        glBindTexture(GL_TEXTURE_2D, 0);

		return image;
	}
};