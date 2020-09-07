#pragma once


/*! @abstract 2-component column vector
 */
struct Vector2 {
    float x, y;
};


/*! @abstract 3-component column vector
 */
struct Vector3 {
    float x, y, z;
};


/*! @abstract 4-component column vector
 */
struct Vector4 {
    float x, y, z, w;
};


/*! @abstract column major format matrix
 */
struct Matrix4 {
    float m11, m21, m31, m41;	// col 1
    float m12, m22, m32, m42;   // col 2
    float m13, m23, m33, m43;   // col 3
    float m14, m24, m34, m44;   // col 4
};


/*! @returns dot product of two Vector3s
 */
inline float dot(Vector3 a, Vector3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}


/*! @returns cross product of two Vector3s
 */
inline Vector3 cross(Vector3 a, Vector3 b) {
    return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}


/*! @returns scalar 4D dot product of two Vector4s
 */
inline float dot4(Vector4 a, Vector4 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}


/*! @abstract 3D Cross product applied to a 4-component vector
 *  @returns Vector4 with cross product of XYZ components, and W=0
 */
inline Vector4 cross4(Vector4 a, Vector4 b) {
    return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 0.0f };
}
