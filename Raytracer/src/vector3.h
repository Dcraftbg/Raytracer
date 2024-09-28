#pragma once
class Vector3f {
public:
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3f(float a) : x(a), y(a), z(a) {}

    Vector3f operator/(float a) const {
        return Vector3f(x/a, y/a, z/a);
    }

    Vector3f operator*(float a) const {
        return Vector3f(x*a, y*a, z*a);
    }
    friend Vector3f operator*(float scalar, const Vector3f& vec) {
        return vec*scalar; 
    }

    Vector3f operator+(Vector3f v2) const {
        return Vector3f(x+v2.x, y+v2.y, z+v2.z);
    }

    Vector3f operator-(Vector3f v2) const {
        return Vector3f(x-v2.x, y-v2.y, z-v2.z);
    }
};



float dot(const Vector3f& v1, const Vector3f& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
