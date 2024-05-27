#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "imgui/imgui.h"

#include <opencv2/opencv.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Helper {
public:
	GLuint textureID;

    GLuint matToTexture(cv::Mat& mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.data);
        glBindTexture(GL_TEXTURE_2D, 0);

        return textureID;
    }

    std::wstring StringToLPCWSTR(const std::string& str) {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstrTo[0], size_needed);
        return wstrTo;
    }
};