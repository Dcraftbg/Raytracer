#pragma once
#include <Windows.h>
#include "result.h"
#include "utils.h"
#include <algorithm>
class Window;
class Framebuffer {
public:
    uint32_t* pixels;
    size_t width;
    size_t height;
private:
    HBITMAP _h_bitmap;
    DEL_COPY(Framebuffer);
public:
    Framebuffer(Framebuffer&& fb) {
        pixels = fb.pixels;
        width = fb.width;
        height = fb.height;
        _h_bitmap = fb._h_bitmap;
        fb.pixels = nullptr;
        fb.width = 0;
        fb.height = 0;
        fb._h_bitmap = nullptr;
    }
    Framebuffer& operator=(Framebuffer&& fb) {
        if(this != &fb) {
            DeleteObject(_h_bitmap);
            pixels = fb.pixels;
            width = fb.width;
            height = fb.height;
            _h_bitmap = fb._h_bitmap;
            fb.pixels = nullptr;
            fb.width = 0;
            fb.height = 0;
            fb._h_bitmap = nullptr;
        }
        return *this;
    }
public:
    Framebuffer(uint32_t* pixels, size_t width, size_t height, HBITMAP h_bitmap) : pixels(pixels), width(width), height(height), _h_bitmap(h_bitmap) {}
    static Result<Framebuffer, int> from_window(const Window& win);
    friend class Window;

    void fill(uint32_t color) {
        std::fill(pixels, pixels+(width*height), color);
    }
    ~Framebuffer() {
        DeleteObject(_h_bitmap);
    }
};
