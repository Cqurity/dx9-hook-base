#include "pch.h"

ImFont* g_font;
ImFont* t_font;

HWND g_hwnd = nullptr;
WNDPROC g_wndproc = nullptr;

DWORD lastTick;
const int InputDelay = 200;

bool renderer_init;
bool IsMenuOpen = true;

bool flag1;
bool flag2;
float my_color[4];

int vKey = VK_END;

MAKE_HOOK<convention_type::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* > PresentEx;
MAKE_HOOK<convention_type::stdcall_t, HRESULT, LPDIRECT3DDEVICE9> EndEx;
MAKE_HOOK<convention_type::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT > PrimitiveEx;
MAKE_HOOK<convention_type::stdcall_t, HRESULT, LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS* > ResetEx;

LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto& io = ImGui::GetIO();

    switch (msg)
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        io.MouseDown[0] = true;
        return true;
    case WM_LBUTTONUP:
        io.MouseDown[0] = false;
        return true;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
        io.MouseDown[1] = true;
        return true;
    case WM_RBUTTONUP:
        io.MouseDown[1] = false;
        return true;
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
        io.MouseDown[2] = true;
        return true;
    case WM_MBUTTONUP:
        io.MouseDown[2] = false;
        return true;
    case WM_XBUTTONDOWN:
    case WM_XBUTTONDBLCLK:
        if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
            io.MouseDown[3] = true;
        else if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
            io.MouseDown[4] = true;
        return true;
    case WM_XBUTTONUP:
        if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
            io.MouseDown[3] = false;
        else if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
            io.MouseDown[4] = false;
        return true;
    case WM_MOUSEWHEEL:
        io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
        return true;
    case WM_MOUSEMOVE:
        io.MousePos.x = (signed short)(lParam);
        io.MousePos.y = (signed short)(lParam >> 16);
        return true;
    case WM_KEYDOWN:
        if (wParam < 256)
            io.KeysDown[wParam] = 1;
        return true;
    case WM_KEYUP:
        if (wParam < 256)
            io.KeysDown[wParam] = 0;
        return true;
    case WM_CHAR:
        if (wParam > 0 && wParam < 0x10000)
            io.AddInputCharacter((unsigned short)wParam);
        return true;
    }

    return 0;
}

LRESULT WINAPI WndProc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    switch (u_msg)
    {
    case WM_KEYDOWN:
        if (w_param == vKey)
            IsMenuOpen = !IsMenuOpen;
        break;
    default:
        break;
    }

    if (IsMenuOpen && ImGui_ImplDX9_WndProcHandler(hwnd, u_msg, w_param, l_param))
        return true;

    return CallWindowProcA(g_wndproc, hwnd, u_msg, w_param, l_param);
}

bool GetKeyPressed(int vKey)
{
    const auto isPressed = GetAsyncKeyState(vKey) & 0x8000 && GetTickCount64() - lastTick > InputDelay;
    if (isPressed)
    {
        lastTick = static_cast<DWORD>(GetTickCount64());
    }

    return isPressed;
}


HRESULT endScene(LPDIRECT3DDEVICE9 pDevice)
{
    HRESULT pp = EndEx.CallOriginal(pDevice);
    return pp;
}

HRESULT reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    const auto hResult = ResetEx.CallOriginal(pDevice, pPresentationParameters);

    if (hResult >= 0)
    {
        ImGui_ImplDX9_CreateDeviceObjects();
    }
	
    return hResult;
}

HRESULT present(LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestinationRect, HWND hDestinationWindow, CONST RGNDATA* pDirtyRegion)
{
    if (!renderer_init) {
        render.init(pDevice);
        renderer_init = true;
    }

    ImGui::CreateContext();
    render.begin_draw();

    if (IsMenuOpen)
    {
        if (ImGui_ImplWin32_Init(g_hwnd) && ImGui_ImplDX9_Init(pDevice))
        {
            ImGui::PushFont(t_font);
            ImGui::Begin("Header", &IsMenuOpen, ImGuiWindowFlags_NoSavedSettings);
            {
                ImGui::PushFont(g_font);
            	
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                        if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                        if (ImGui::MenuItem("Close", "Ctrl+W")) {}
                    	
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }
            	
                ImGui::BeginChild("##child", ImVec2(450.0f, 100.0f), true, ImGuiWindowFlags_NoSavedSettings);
                {
                    ImGui::Checkbox("flag1", &flag1);
                    ImGui::Checkbox("flag2", &flag2);
                }
                ImGui::EndChild();

                // Edit a color (stored as ~4 floats)
                ImGui::ColorEdit4("Color", my_color);

                // Plot some values
                const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
                ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

                // Display contents in a scrolling region
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
                ImGui::BeginChild("Scrolling");
                for (int n = 0; n < 50; n++)
                    ImGui::Text("%04d: Some text", n);
                ImGui::EndChild();
            }
            ImGui::End();
        }
    }
	
    render.end_draw();
    HRESULT hres = PresentEx.CallOriginal(pDevice, pSourceRect, pDestinationRect, hDestinationWindow, pDirtyRegion);
    return hres;
}

void OnAttach()
{
    const auto lpWindowName = "Target Window Caption";
	
    g_hwnd = FindWindowA(nullptr, lpWindowName);
    g_wndproc = WNDPROC(SetWindowLongA(g_hwnd, GWL_WNDPROC, LONG_PTR(WndProc)));
	
    DWORD dxd9 = 0;
    DWORD d3dOffset = 0;

    const auto pattern = PBYTE("\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86");
    const auto dxd9dll = L"d3d9.dll";
    const auto mask = "xx????xx????xx";
    const DWORD length = 0x128000;

    do dxd9 = reinterpret_cast<DWORD>(GetModuleHandle(dxd9dll));
    while (dxd9 == 0);

    do d3dOffset = Util::FindPattern(dxd9, length, pattern, mask);
    while (d3dOffset == 0);

    DWORD* vTable;
    memcpy(&vTable, reinterpret_cast<void*>(d3dOffset + 2), 4);

    if (!vTable) return;

    ResetEx.Apply(vTable[16], [](LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) -> HRESULT { return reset(pDevice, pPresentationParameters); });
    PresentEx.Apply(vTable[17], [](LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestinationRect, HWND hDestinationWindow, CONST RGNDATA* pDirtyRegion) -> HRESULT
        { return present(pDevice, pSourceRect, pDestinationRect, hDestinationWindow, pDirtyRegion); });

    EndEx.Apply(vTable[42], [](LPDIRECT3DDEVICE9 pDevice) -> HRESULT { return endScene(pDevice); });
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    UNREFERENCED_PARAMETER(lpReserved);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        DisableThreadLibraryCalls(hModule);
        OnAttach();

        break;

    case DLL_THREAD_ATTACH:
        return TRUE;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    default:;
    }
    return TRUE;
}


