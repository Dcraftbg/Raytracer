#include <stdio.h>
#include <Windows.h>
#include <stdint.h>
#include <time.h>
#include <memory>
#include <assert.h>
#include "winbuilder.h"
#include "window.h"
#include "framebuffer.h"
#include "vector3.h"
uint32_t rand_color() {
    return (rand() << 4) ^ (rand() << 8) ^ (rand() << 12);
}
struct Ray {
    Vector3f direction;
    Vector3f origin;
};
struct Viewport {
    float width, height;
    Vector3f viewport_u, viewport_v;
};
class Camera {
public:
    Vector3f eyepoint;
    float focal;
    Camera() : eyepoint(0), focal(1.0) {}
};
uint32_t vec3_to_rgb(Vector3f c) {
    return (0xFF << 24) | (uint32_t(c.r*255.f) << 16) | (uint32_t(c.g*255.f) << 8) | (uint32_t(c.b*255.f));
}
float prev=0.0;
#include <math.h>
struct Sphere {
    Vector3f center;
    float radius;
    uint32_t color;

    Sphere(
        Vector3f center,
        float radius,
        uint32_t color
    ) : center(center), radius(radius), color(color) {}
};

struct HitRecord {
    Vector3f point;
    Vector3f normal;
    float t;
};
bool hit_sphere(const Sphere& sphere, const Ray& ray, float ray_min, float ray_max, HitRecord& record) {
    Vector3f op = sphere.center-ray.origin;
    float a = dot(ray.direction, ray.direction);
    float h = dot(ray.direction, op);
    float c = dot(op, op) - sphere.radius*sphere.radius;
    float D = h*h - a*c;
    if(D < 0) {
        return false;
    }
    float sqrtd = sqrtf(D);
    float root = (h - sqrtd) / a;
    if(root <= ray_min || root >= ray_max) {
        root = (h + sqrtd) / a;
        if(root <= ray_min || root >= ray_max) return false;
    }
    record.t = root;
    record.point = ray.origin + ray.direction*record.t;
    record.normal = (record.point - sphere.center) / sphere.radius;

    return true;
}
// Stolen from raylib
uint32_t color_brightness(uint32_t color, float factor)
{
    if (factor > 1.0f) factor = 1.0f;
    else if (factor < -1.0f) factor = -1.0f;

    float red   = float((color >> 16) & 0xFF);
    float green = float((color >>  8) & 0xFF);
    float blue  = float((color      ) & 0xFF);

    if (factor < 0.0f)
    {
        factor = 1.0f + factor;
        red *= factor;
        green *= factor;
        blue *= factor;
    }
    else
    {
        red = (255 - red)*factor + red;
        green = (255 - green)*factor + green;
        blue = (255 - blue)*factor + blue;
    }
    return (uint32_t(red) << 16) | (uint32_t(green) << 8) | (uint32_t(blue));
}
uint32_t ray_color(const Ray& ray) {
    static Sphere spheres[] = {
        Sphere(Vector3f(0.0f,0.0f ,-1.0f), 0.5f, 0xFFFF0000),
        Sphere(Vector3f(0.0f,-10.0f,-1.0f), 9.5f, 0xFF00FF00),
    };
    for(size_t i = 0; i < (sizeof(spheres)/sizeof(*spheres)); ++i) {
        const Sphere& s = spheres[i];
        HitRecord record;
        
        if(hit_sphere(s, ray, 0.1f, 5.0f, record)) {
            float t = (record.normal.x + record.normal.y + record.normal.z) / 3.0f;
            float n =
                // linear
                t;
                // sin
                // sinf(t);
                // ease in out Quint
                // t < 0.5f 
                //    ? 16.0f * t * t * t * t * t
                //    : 1.0f  - powf(-2.0f * t + 2.0f, 5.0f) / 2.0f;
                // ease in Cubic
                // t*t*t;
                // ease in Circ
                // 1.0f - sqrtf(1.0f - powf(t, 2.0f));
                // ease out quad
                // 1.0f - powf(1.0f - t, 3);
            float unit = n * 2.0f - 1.0f;
            return color_brightness(
                s.color,
                unit
            );
        }
    }
    float y = 0.5f*(ray.direction.y + 1.0f);
    Vector3f color = (1.0f-y)*Vector3f(1.0f) + y*Vector3f(0.5f, 0.7f, 1.0f);
    return vec3_to_rgb(color);
}


struct GameState {
    Camera camera;
    Viewport viewport;
    GameState() {}
};
void draw(Window& window) {
    if(window.height > 0) {
        auto fb_result(std::move(Framebuffer::from_window(window)));
        assert(!fb_result.is_error);

        auto fb(std::move(fb_result.unwrap));
        GameState& gm = *((GameState*)window.user);

        Vector3f pixel_delta_u = gm.viewport.viewport_u / float(window.width);
        Vector3f pixel_delta_v = gm.viewport.viewport_v / float(window.height);

        Vector3f vp = gm.camera.eyepoint - Vector3f(0, 0, gm.camera.focal) - (gm.viewport.viewport_u + gm.viewport.viewport_v)*0.5f;
        Vector3f pixel0 = vp + (pixel_delta_u + pixel_delta_v)*0.5f;

        for(size_t y = 0; y < window.height; ++y) {
            for(size_t x = 0; x < window.width; ++x) {
                Ray ray;
                
                Vector3f pixel_center = pixel0 + (float(x) * pixel_delta_u) + (float(y) * pixel_delta_v);
                Vector3f direction = pixel_center - gm.camera.eyepoint;

                ray.origin = gm.camera.eyepoint;
                ray.direction = direction;

                fb.pixels[y*window.width+x] = ray_color(ray);
            }
        }
        window.draw(fb);
    }
}

void on_resize(Window* window, size_t width, size_t height) {
    window->width = width;
    window->height = height;
    GameState& gm = *((GameState*)window->user);
    gm.viewport.height = 2.0;
    gm.viewport.width = gm.viewport.height * (float(window->width)/float(window->height));

    gm.viewport.viewport_u = Vector3f(gm.viewport.width, 0, 0);
    gm.viewport.viewport_v = Vector3f(0, -gm.viewport.height, 0);
    draw(*window);
}
#define W_RATIO 16
#define H_RATIO 9
#define SCALAR  100
#define WIDTH   (W_RATIO*SCALAR)
#define HEIGHT  (H_RATIO*SCALAR)
int main(void) {
    srand((unsigned int)time(NULL));
    auto windowResult = 
        WindowBuilder()
            .set_title("Hello from C++")
            .set_size(WIDTH, HEIGHT)
            .create();
    if(windowResult.is_error) {
        fprintf(stderr, "ERROR: Failed to create window: %04X\n", windowResult.unwrap_err);
        return 1;
    }

    std::unique_ptr<Window> window(windowResult.unwrap);
    std::unique_ptr<GameState> state(new GameState());
    window->set_user_pointer((void*)state.get());
    window->handlers.resize = on_resize;
    window->show();
    window->event_loop();
    return 0;
#if 0
    srand(time(NULL));
    HINSTANCE instance = GetModuleHandle(NULL);
    const char window_name[] = "Hello from C++";
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = window_proc;
    wc.hInstance = instance;
    wc.lpszClassName = window_name;

    RegisterClass(&wc);

    HWND window = CreateWindowEx(
        0, 
        window_name,
        "C++ Program for windows",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        instance,
        NULL
    );
    if(!window) {
        fprintf(stderr, "ERROR: Failed to create a window\n");
        return 1;
    }
    draw(window);
    ShowWindow(window, SW_NORMAL);
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
#endif
}
