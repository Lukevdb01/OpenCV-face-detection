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

void GuiRenderer::Render(cv::Mat image, Helper* helper)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpaceOverViewport(dockspace_id);

	ImGui::Begin("Hello, world!");
	ImGui::Image((void*)(intptr_t)helper->textureID, ImVec2(image.cols, image.rows));
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
