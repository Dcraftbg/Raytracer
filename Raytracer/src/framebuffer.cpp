#include "framebuffer.h"
#include "window.h"

Result<Framebuffer, int> Framebuffer::from_window(const Window& win) {
    HDC hdc = GetDC(win.get_handle());
    if(!hdc) return GetLastError();
    BITMAPINFO bmi = {0};
    int width = (int)win.width;
    int height = (int)win.height;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    uint32_t* fb = nullptr;
    HBITMAP bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&fb, NULL, 0);
    ReleaseDC(win.get_handle(), hdc);
    if(!bitmap) return GetLastError();
    return Framebuffer(fb, width, height, bitmap);
}
