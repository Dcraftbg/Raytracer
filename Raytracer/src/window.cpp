#pragma once
#include "window.h"
#include "framebuffer.h"
void Window::event_loop() {
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT Window::on_system_event(UINT message, WPARAM msg_argw, LPARAM msg_argl) {
    switch(message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        DestroyWindow(m_handle);
        return 0;
    case WM_SIZE:
        UINT width = LOWORD(msg_argl);
        UINT height = HIWORD(msg_argl);
        handlers.resize(this, width, height);
        return 0;
    }
    return DefWindowProc(m_handle, message, msg_argw, msg_argl);
}

void Window::draw(const Framebuffer& fb) {
    HDC hdc = GetDC(m_handle);
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, fb._h_bitmap);
    BitBlt(hdc, 0, 0, (int)width, (int)height, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);
    ReleaseDC(m_handle, hdc);
}
