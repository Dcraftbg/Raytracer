#pragma once
#include <Windows.h>
class Window;
struct EventHandlers {
    void (*resize)(Window* window, size_t width, size_t height);
};

class Framebuffer;
class Window {
private:
    HWND m_handle;
public:
    size_t width;
    size_t height;
    const char* title;
    EventHandlers handlers;
    void* user;
public:
    Window(HWND handle, size_t width, size_t height, const char* title) : m_handle(handle), width(width), height(height), title(title) {}
    ~Window() { DestroyWindow(m_handle); }

    void set_user_pointer(void* ptr) {
        user=ptr;
    }
    void draw(const Framebuffer&);
    inline HWND get_handle() const {
        return m_handle;
    }
    inline void show() {
        ShowWindow(m_handle, SW_NORMAL);
    }
    void event_loop();
    LRESULT on_system_event(UINT message, WPARAM msg_argw, LPARAM msg_argl);
};
