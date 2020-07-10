#include "pch.h"
#include <cmath>
#include "c_renderer.hpp"

#include "imgui/imgui_internal.h"
#include "imgui/dx9/imgui_impl_dx9.h"
#include "imgui/win32/imgui_impl_win32.h"

extern ImFont* g_font;
extern ImFont* t_font;

c_renderer render;

void c_renderer::init(LPDIRECT3DDEVICE9 device)
{
	if (!device)
		return;

	if (!m_device)
		m_device = device;

	ImGui::CreateContext();

	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX9_Init(device);

	if (!m_default)
		m_default = ImGui::GetIO().Fonts->AddFontDefault();
	/* ImGui::GetIO().Fonts->AddFontFromFileTTF("path_to_font", 14.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()); */


	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	//style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	//style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	ImGuiIO io = ImGui::GetIO();
	
	io.Fonts->AddFontDefault();

	g_font = io.Fonts->AddFontFromFileTTF((R"(C:\Windows\Fonts\Ruda-Bold.ttf)"), 16);
	t_font = io.Fonts->AddFontFromFileTTF((R"(C:\Windows\Fonts\Ruda-Bold.ttf)"), 26);
	
	/*io.Fonts->AddFontFromFileTTF((R"(C:\Windows\Fonts\Ruda-Bold.ttf)"), 12);
	io.Fonts->AddFontFromFileTTF((R"(C:\Windows\Fonts\Ruda-Bold.ttf)"), 14);
	io.Fonts->AddFontFromFileTTF((R"(C:\Windows\Fonts\Ruda-Bold.ttf)"), 18);*/
}

void c_renderer::free(void)
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

void c_renderer::begin_draw(void)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;

	ImGui::GetStyle().AntiAliasedFill = true;
	ImGui::GetStyle().AntiAliasedLines = true;

	ImGui::Begin("##overlay", nullptr, flags);
	ImGui::PushFont(g_font);

	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

	m_width = ImGui::GetIO().DisplaySize.x;
	m_height = ImGui::GetIO().DisplaySize.y;
}

void c_renderer::end_draw(void)
{
	ImGui::GetOverlayDrawList()->PushClipRectFullScreen();

	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void c_renderer::draw_text(float_t x, float_t y, const char* text, bool outlined, ImColor color, e_flags flags, ImFont* font, ...)
{
	switch (flags)
	{
	case c_renderer::text_normal:

		if (outlined)
		{
			ImGui::GetWindowDrawList()->AddText(ImVec2(x, y + 1.0f), ImColor(0, 0, 0, 255), text);
			ImGui::GetWindowDrawList()->AddText(ImVec2(x + 1.0f, y), ImColor(0, 0, 0, 255), text);
		}

		ImGui::GetWindowDrawList()->AddText(ImVec2(x, y), color, text);
		break;
	case c_renderer::text_with_font:

		if (outlined)
		{
			ImGui::GetWindowDrawList()->AddText(font, font->FontSize, ImVec2(x, y + 1.0f), ImColor(0, 0, 0, 255), text);
			ImGui::GetWindowDrawList()->AddText(font, font->FontSize, ImVec2(x + 1.0f, y), ImColor(0, 0, 0, 255), text);
		}

		ImGui::GetWindowDrawList()->AddText(font, font->FontSize, ImVec2(x, y), color, text);
		break;
	default:
		break;
	}
}

void c_renderer::draw_line(float_t x1, float_t y1, float_t x2, float_t y2, ImColor color, float_t thickness)
{
	ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}

void c_renderer::draw_rect(float_t x, float_t y, float_t w, float_t h, ImColor color, e_flags flags, float_t rounding, uintptr_t points, float_t thickness)
{
	switch (flags)
	{
	case c_renderer::rect_normal:
		ImGui::GetWindowDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding, points, thickness);
		break;
	case c_renderer::rect_filled:
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding, points);
		break;
	default:
		break;
	}
}

void c_renderer::draw_triangle(float_t x1, float_t y1, float_t x2, float_t y2, float_t x3, float_t y3, ImColor color, e_flags flags, float_t thickness)
{
	switch (flags)
	{
	case c_renderer::rect_normal:
		ImGui::GetWindowDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color, thickness);
		break;
	case c_renderer::rect_filled:
		ImGui::GetWindowDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color);
		break;
	default:
		break;
	}
}

void c_renderer::draw_circle(float_t x, float_t y, float_t radius, ImColor color, e_flags flags, uintptr_t points, float_t thickness)
{
	switch (flags)
	{
	case circle_normal:
		ImGui::GetWindowDrawList()->AddCircle(ImVec2(x, y), radius, color, points, thickness);
		break;
	case circle_filled:
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(x, y), radius, color, points);
		break;
	case circle_3d:
		break;
	default:
		break;
	}
}
