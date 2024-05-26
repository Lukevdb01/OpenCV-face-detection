#include "gui_renderer.h"

void GuiRenderer::Initialize(HWND hwnd, HDC hdc, HGLRC hglrc)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
}

void GuiRenderer::Render(cv::Mat image, Helper* helper, std::vector<cv::Rect> detections)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpaceOverViewport(dockspace_id);

	ImGui::Begin("Navigation");
	ImGui::Text("Number of people detected: %d", detections.size());
	for (int i = 0; i < detections.size(); i++)
	{
		// Create a collapsible header for each detection
		if (ImGui::CollapsingHeader(("Detection " + std::to_string(i + 1)).c_str()))
		{
			ImGui::Text("X: %d", detections[i].x);
			ImGui::Text("Y: %d", detections[i].y);
			ImGui::Text("Width: %d", detections[i].width);
			ImGui::Text("Height: %d", detections[i].height);
		}
	}
	ImGui::End();

	ImGui::Begin("Hello, world!");
	GLuint textureID = helper->matToTexture(image, GL_LINEAR, GL_LINEAR, GL_CLAMP);

	ImGui::Image((void*)(intptr_t)textureID, ImGui::GetContentRegionAvail());
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GuiRenderer::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
