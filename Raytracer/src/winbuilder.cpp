#include "winbuilder.h"
Result<Window*, DWORD> WindowBuilder::create() {
    HINSTANCE instance = GetModuleHandle(nullptr);
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = _window_proc;
    wc.hInstance = instance;
    wc.lpszClassName = title;
    if(!RegisterClassA(&wc)) return GetLastError();
    HWND handle = CreateWindowExA(
        0, 
        title,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr,
        nullptr,
        instance,
        nullptr
    );
    if(!handle) return GetLastError();
    auto window = new Window(handle, width, height, title);
    SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)window);
    return window;
}
