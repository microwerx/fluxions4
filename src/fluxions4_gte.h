#pragma once

struct Vector2 {
    float x, y;
};

struct Vector3 {
    float x, y, z;
};

struct Vector4 {
    float x, y, z, w;
};

struct Matrix4 {
    float m11, m21, m31, m41;	// col 1
    float m12, m22, m32, m42;   // col 2
    float m13, m23, m33, m43;   // col 3
    float m14, m24, m34, m44;   // col 4
};

inline float dot(Vector3 a, Vector3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vector3 cross(Vector3 a, Vector3 b) {
    return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

