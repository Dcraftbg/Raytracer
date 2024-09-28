#pragma once
#include <Windows.h>
#include "result.h"
#include "window.h"
#include <utility>
class WindowBuilder {
private:
    int width;
    int height;
    const char* title;
public:
    inline WindowBuilder() : width(CW_USEDEFAULT), height(CW_USEDEFAULT), title(nullptr) {}
    inline WindowBuilder& set_title(const char* title) {
        this->title = title;
        return *this;
    }
    inline WindowBuilder& set_size(int width, int height) {
        this->width = width;
        this->height = height;
        return *this;
    }

    Result<Window*, DWORD> create();
private:
    static LRESULT _window_proc(HWND window, UINT message, WPARAM msg_argw, LPARAM msg_argl) {
        auto window_ptr = (Window*)GetWindowLongPtr(window, GWLP_USERDATA);
        if(!window_ptr) return DefWindowProc(window, message, msg_argw, msg_argl);
        return window_ptr->on_system_event(message, msg_argw, msg_argl);
    }
};
