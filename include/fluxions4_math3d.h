#ifndef FLUXIONS_GTE_BASE_HPP
#define FLUXIONS_GTE_BASE_HPP


#include <math.h>


////////////////////////////////////////////////////////////
// 32-bit Constants ////////////////////////////////////////
////////////////////////////////////////////////////////////

#define FX_F32_SQRT2                          1.41421356f
#define FX_F32_SQRT3                          1.73205080f
#define FX_F32_SQRT5                          2.23606797f
#define FX_F32_ONEOVERSQRT2                   0.70710678f
#define FX_F32_ONEOVERSQRT3                   0.57735026f
#define FX_F32_ONEOVERSQRT5                   0.44721359f
#define FX_F32_E                              2.71828182f
#define FX_F32_PHI                            1.61803398f
#define FX_F32_TWOPI                          6.28318530f
#define FX_F32_PI                             3.1415926535f
#define FX_F32_PIOVERTWO                      1.57079632f
#define FX_F32_PIOVERTHREE                    1.04719755f
#define FX_F32_PIOVERFOUR                     0.78539816f
#define FX_F32_RADIANS_TO_DEGREES            57.29577951f
#define FX_F32_EXTRA_RADIANS_TO_HOURS         3.81971863f
#define FX_F32_EXTRA_RADIANS_TO_ARCMINS     229.18311805f
#define FX_F32_EXTRA_RADIANS_TO_ARCSECS   13750.98708313f
#define FX_F32_DEGREES_TO_RADIANS             0.01745329f
#define FX_F32_EXTRA_DEGREES_TO_HOURS         0.06666666f
#define FX_F32_EXTRA_DEGREES_TO_ARCMINS       4.0f
#define FX_F32_EXTRA_DEGREES_TO_ARCSECS     240.0f
#define FX_F32_HOURS_TO_RADIANS               0.26179938f
#define FX_F32_HOURS_TO_DEGREES              15.0f
#define FX_F32_HOURS_TO_ARCMINS              60.0f
#define FX_F32_HOURS_TO_ARCSECS            3600.0f
#define FX_F32_ARCMINS_TO_RADIANS             0.00436332f
#define FX_F32_ARCMINS_TO_DEGREES             0.00416666f
#define FX_F32_ARCMINS_TO_HOURS               0.01666666f
#define FX_F32_ARCMINS_TO_ARCSECS            60.0f
#define FX_F32_ARCSECS_TO_DEGREES             2.77777777e-4f
#define FX_F32_ARCSECS_TO_RADIANS             4.84813681e-6f
#define FX_F32_ARCSECS_TO_HOURS               2.77777777e-4f
#define FX_F32_ARCSECS_TO_ARCMINS             0.01666666f
#define FX_F32_C                      299792458.0f            // speed of light
#define FX_F32_G                              6.6740831e-11f  // Newton's constant of gravitation
#define FX_F32_H                              6.62607004e-34f // Planck's h constant
#define FX_F32_HBAR                           1.05457180e-34f // Planck's h bar constant
#define FX_F32_EPSILON0                       8.85418781e-12f // electric constant

#define FX_F32_LOG2E                          1.44269504f     // log2(e)
#define FX_F32_LOG10E                         0.43429448f     // log10(e)
#define FX_F32_LN2                            0.69314718f     // ln(2)
#define FX_F32_LN10                            2.30258509f    // ln(10)
#define FX_F32_PI_2                            1.57079632f    // pi/2
#define FX_F32_PI_4                            0.78539816f    // pi/4
#define FX_F32_1_PI                            0.31830988f    // 1/pi
#define FX_F32_2_PI                            0.63661977f    // 2/pi
#define FX_F32_1_2PI                           0.159154943f   // 1/(2pi)
#define FX_F32_2_SQRTPI                        1.12837916f    // 2/sqrt(pi)
#define FX_F32_SQRT1_2                         0.70710678f    // 1/sqrt(2)


////////////////////////////////////////////////////////////
// SCALAR MATH /////////////////////////////////////////////
////////////////////////////////////////////////////////////


static inline float
clampf(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}


////////////////////////////////////////////////////////////
// COLOR MATH //////////////////////////////////////////////
////////////////////////////////////////////////////////////


/*
** RGB color
*/
struct col3 {
	float r;
	float g;
	float b;
};


/*
** RGBA color
*/
struct col4 {
	float r;
	float g;
	float b;
	float a;
};


/*
** Construct an RGB color
*/
static inline struct col3
col3_make(float r, float g, float b) {
	struct col3 c = { r, g, b };
	return c;
}


/*
** Copy an RGB color
*/
static inline struct col3
col3_copy(struct col3 C) {
	return col3_make(C.r, C.g, C.b);
}


/*
** Clamp an RGB color to 0 and 1
*/
static inline struct col3
col3_saturate(struct col3 color) {
	return col3_make(
		clampf(color.r, 0.0f, 1.0f),
		clampf(color.g, 0.0f, 1.0f),
		clampf(color.b, 0.0f, 1.0f)
	);
}


/*
** Construct an RGBA color
*/
static inline struct col4
col4_make(float r, float g, float b, float a) {
	struct col4 c = { r, g, b, a };
	return c;
}


/*
** Copy an RGBA color
*/
static inline struct col4
col4_copy(struct col4 C) {
	return col4_make(C.r, C.g, C.b, C.a);
}


/*
** Clamp an RGBA color to 0 and 1
*/
static inline struct col4
col4_saturate(struct col4 color) {
	return col4_make(
		clampf(color.r, 0.0f, 1.0f),
		clampf(color.g, 0.0f, 1.0f),
		clampf(color.b, 0.0f, 1.0f),
		clampf(color.a, 0.0f, 1.0f)
	);
}


////////////////////////////////////////////////////////////
// VECTORS /////////////////////////////////////////////////
////////////////////////////////////////////////////////////


struct vec3 {
#ifdef __cplusplus
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
#else
	float x;
	float y;
	float z;
#endif
};


/*
** Make a vector from (x, y, z)
*/
static inline struct vec3
vec3_make(float x, float y, float z) {
	struct vec3 v = { x, y, z };
	return v;
}


/*
** Make a vector from a scalar
*/
static inline struct vec3
vec3_scalar(float x) {
	struct vec3 v = { x, x, x };
	return v;
}


/*
** Return a zero vector
*/
static inline struct vec3
vec3_zero() {
	return vec3_scalar(0.0f);
}


/*
** Return component-wise vector negative (-v)
*/
static inline struct vec3
vec3_neg(struct vec3 v) {
	return vec3_make(-v.x, -v.y, -v.z);
}


/*
** Return component-wise vector addition (a + b)
*/
static inline struct vec3
vec3_add(struct vec3 a, struct vec3 b) {
	return vec3_make(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	);
}


/*
** Return component-wise vector subtraction (a - b)
*/
static inline struct vec3
vec3_sub(struct vec3 a, struct vec3 b) {
	return vec3_make(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	);
}


/*
** Return fused-multiply-add of a, b, c vectors
*/
static inline struct vec3
vec3_fma(struct vec3 a, struct vec3 b, struct vec3 c) {
	return vec3_make(
		fmaf(a.x, b.x, c.x),
		fmaf(a.y, b.y, c.y),
		fmaf(a.z, b.z, c.z)
	);
}


/*
** Return product of scalar s and vector v
*/
static inline struct vec3
vec3_scale(float s, struct vec3 v) {
	return vec3_make(
		s * v.x,
		s * v.y,
		s * v.z
	);
}


/*
** Return length of vector v
*/
static inline float
length(struct vec3 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}


/*
** Return unit vector of vector v
*/
static inline struct vec3
normalize(struct vec3 v) {
	float mag = length(v);
	if (mag == 0.0f) return vec3_zero();
	return vec3_scale(1.0f / mag, v);
}


/*
** Return normalized number (-1 to 1) in range (0, 1)
*/
static inline struct vec3
standardize(struct vec3 v) {
	return vec3_make(
		fmaf(v.x, 0.5f, 0.5f),
		fmaf(v.y, 0.5f, 0.5f),
		fmaf(v.z, 0.5f, 0.5f)
	);
}


/*
** Return dot product of a and b
*/
static inline float
dot(struct vec3 a, struct vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


/*
** Return cross product of a and b
*/
static inline struct vec3
cross(struct vec3 a, struct vec3 b) {
	return vec3_make(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}


struct mat4 {
#ifdef __cplusplus
	float m11 = 1.0f, m21 = 0.0f, m31 = 0.0f, m41 = 0.0f;
	float m12 = 0.0f, m22 = 1.0f, m32 = 0.0f, m42 = 0.0f;
	float m13 = 0.0f, m23 = 0.0f, m33 = 1.0f, m43 = 0.0f;
	float m14 = 0.0f, m24 = 0.0f, m34 = 0.0f, m44 = 1.0f;

#else
	float m11, m21, m31, m41;
	float m12, m22, m32, m42;
	float m13, m23, m33, m43;
	float m14, m24, m34, m44;
#endif

#ifdef __cplusplus
	mat4() {}

	mat4(float a11, float a12, float a13, float a14,
		 float a21, float a22, float a23, float a24,
		 float a31, float a32, float a33, float a34,
		 float a41, float a42, float a43, float a44) :
		m11(a11), m21(a21), m31(a31), m41(a41),
		m12(a12), m22(a22), m32(a32), m42(a42),
		m13(a13), m23(a23), m33(a33), m43(a43),
		m14(a14), m24(a24), m34(a34), m44(a44) {}
#endif
};


/*
** Create a 4x4 matrix in row major format
*/
static inline struct mat4
mat4_make(
	float a11, float a12, float a13, float a14,
	float a21, float a22, float a23, float a24,
	float a31, float a32, float a33, float a34,
	float a41, float a42, float a43, float a44) {
	struct mat4 m = {
		a11, a21, a31, a41,
		a12, a22, a32, a42,
		a13, a23, a33, a43,
		a14, a24, a34, a44
	};
	return m;
}


/*
** Create an identity matrix
*/
static inline struct mat4
mat4_identity() {
	return mat4_make(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}


/*
** Create a matrix from a scalar
*/
static inline struct mat4
mat4_scalar(float s) {
	return mat4_make(
		s, s, s, s,
		s, s, s, s,
		s, s, s, s,
		s, s, s, s);
}


/*
** Return copy of 4x4 matrix
*/
static inline struct mat4
mat4_copy(struct mat4 M) {
	return mat4_make(
		M.m11, M.m12, M.m13, M.m14,
		M.m21, M.m22, M.m23, M.m24,
		M.m31, M.m32, M.m33, M.m34,
		M.m41, M.m42, M.m43, M.m44
	);
}


/*
** Return transpose of matrix
*/
static inline struct mat4
mat4_transpose(struct mat4 M) {
	return mat4_make(
		M.m11, M.m21, M.m31, M.m41,
		M.m12, M.m22, M.m32, M.m42,
		M.m13, M.m23, M.m33, M.m43,
		M.m14, M.m24, M.m34, M.m44
	);
}


/*
** Return determinant of matrix
*/
static inline float
mat4_det(struct mat4 M) {
	float t1 = M.m31 * M.m42 - M.m32 * M.m41;
	float t2 = M.m31 * M.m43 - M.m33 * M.m41;
	float t3 = M.m32 * M.m43 - M.m33 * M.m42;
	float t4 = M.m31 * M.m44 - M.m34 * M.m41;
	float t5 = M.m32 * M.m44 - M.m34 * M.m42;
	float t6 = M.m33 * M.m44 - M.m34 * M.m43;
	return M.m11 * (M.m22 * t6 - M.m23 * t5 + M.m24 * t3) -
		M.m12 * (M.m21 * t6 - M.m23 * t4 + M.m24 * t2) +
		M.m13 * (M.m21 * t5 - M.m22 * t4 + M.m24 * t1) -
		M.m14 * (M.m21 * t3 - M.m22 * t2 + M.m23 * t1);
}


/*
** Returns quick inverse of matrix
*/
static inline struct mat4
mat4_quick_inverse(struct mat4 M) {
	return mat4_make(
		M.m11, M.m21, M.m31, -M.m14,
		M.m12, M.m22, M.m32, -M.m24,
		M.m13, M.m23, M.m33, -M.m34,
		0.0f, 0.0f, 0.0f, 1.0f);
}


/*
** Returns inverse of matrix
*/
static inline struct mat4
mat4_inverse(struct mat4 M) {
	float t1 = M.m32 * M.m43 - M.m33 * M.m42;
	float t2 = M.m32 * M.m44 - M.m34 * M.m42;
	float t3 = M.m33 * M.m44 - M.m34 * M.m43;
	float t4 = M.m22 * t3 - M.m23 * t2 + M.m24 * t1;
	float t5 = M.m31 * M.m42 - M.m32 * M.m41;
	float t6 = M.m31 * M.m43 - M.m33 * M.m41;
	float t7 = -M.m21 * t1 + M.m22 * t6 - M.m23 * t5;
	float t8 = M.m31 * M.m44 - M.m34 * M.m41;
	float t9 = M.m21 * t2 - M.m22 * t8 + M.m24 * t5;
	float t10 = -M.m21 * t3 + M.m23 * t8 - M.m24 * t6;
	float t11 = 1.0f / (M.m11 * t4 + M.m12 * t10 + M.m13 * t9 + M.m14 * t7);
	float t12 = M.m22 * M.m43 - M.m23 * M.m42;
	float t13 = M.m22 * M.m44 - M.m24 * M.m42;
	float t14 = M.m23 * M.m44 - M.m24 * M.m43;
	float t15 = M.m22 * M.m33 - M.m23 * M.m32;
	float t16 = M.m22 * M.m34 - M.m24 * M.m32;
	float t17 = M.m23 * M.m34 - M.m24 * M.m33;
	float t18 = M.m21 * M.m43 - M.m23 * M.m41;
	float t19 = M.m21 * M.m44 - M.m24 * M.m41;
	float t20 = M.m21 * M.m33 - M.m23 * M.m31;
	float t21 = M.m21 * M.m34 - M.m24 * M.m31;
	float t22 = M.m21 * M.m42 - M.m22 * M.m41;
	float t23 = M.m21 * M.m32 - M.m22 * M.m31;

	return mat4_make(
		t4 * t11,
		(-M.m12 * t3 + M.m13 * t2 - M.m14 * t1) * t11,
		(M.m12 * t14 - M.m13 * t13 + M.m14 * t12) * t11,
		(-M.m12 * t17 + M.m13 * t16 - M.m14 * t15) * t11,
		t10 * t11,
		(M.m11 * t3 - M.m13 * t8 + M.m14 * t6) * t11,
		(-M.m11 * t14 + M.m13 * t19 - M.m14 * t18) * t11,
		(M.m11 * t17 - M.m13 * t21 + M.m14 * t20) * t11,
		t9 * t11,
		(-M.m11 * t2 + M.m12 * t8 - M.m14 * t5) * t11,
		(M.m11 * t13 - M.m12 * t19 + M.m14 * t22) * t11,
		(-M.m11 * t16 + M.m12 * t21 - M.m14 * t23) * t11,
		t7 * t11,
		(M.m11 * t1 - M.m12 * t6 + M.m13 * t5) * t11,
		(-M.m11 * t12 + M.m12 * t18 - M.m13 * t22) * t11,
		(M.m11 * t15 - M.m12 * t20 + M.m13 * t23) * t11);
}


/*
** Return matrix product of A and B
*/
static inline struct mat4
mat4_mult(struct mat4 A, struct mat4 B) {
	return mat4_make(
		(A.m14 * B.m41 + A.m13 * B.m31 + A.m12 * B.m21 + A.m11 * B.m11),
		(A.m14 * B.m42 + A.m13 * B.m32 + A.m12 * B.m22 + A.m11 * B.m12),
		(A.m14 * B.m43 + A.m13 * B.m33 + A.m12 * B.m23 + A.m11 * B.m13),
		(A.m14 * B.m44 + A.m13 * B.m34 + A.m12 * B.m24 + A.m11 * B.m14),

		(A.m24 * B.m41 + A.m23 * B.m31 + A.m22 * B.m21 + A.m21 * B.m11),
		(A.m24 * B.m42 + A.m23 * B.m32 + A.m22 * B.m22 + A.m21 * B.m12),
		(A.m24 * B.m43 + A.m23 * B.m33 + A.m22 * B.m23 + A.m21 * B.m13),
		(A.m24 * B.m44 + A.m23 * B.m34 + A.m22 * B.m24 + A.m21 * B.m14),

		(A.m34 * B.m41 + A.m33 * B.m31 + A.m32 * B.m21 + A.m31 * B.m11),
		(A.m34 * B.m42 + A.m33 * B.m32 + A.m32 * B.m22 + A.m31 * B.m12),
		(A.m34 * B.m43 + A.m33 * B.m33 + A.m32 * B.m23 + A.m31 * B.m13),
		(A.m34 * B.m44 + A.m33 * B.m34 + A.m32 * B.m24 + A.m31 * B.m14),

		(A.m44 * B.m41 + A.m43 * B.m31 + A.m42 * B.m21 + A.m41 * B.m11),
		(A.m44 * B.m42 + A.m43 * B.m32 + A.m42 * B.m22 + A.m41 * B.m12),
		(A.m44 * B.m43 + A.m43 * B.m33 + A.m42 * B.m23 + A.m41 * B.m13),
		(A.m44 * B.m44 + A.m43 * B.m34 + A.m42 * B.m24 + A.m41 * B.m14)
	);
}


/*
** Component-wise matrix addition
*/
static inline struct mat4
mat4_add(struct mat4 A, struct mat4 B) {
	return mat4_make(
		A.m11 + B.m11, A.m12 + B.m12, A.m13 + B.m13, A.m14 + B.m14,
		A.m21 + B.m21, A.m22 + B.m22, A.m23 + B.m23, A.m24 + B.m24,
		A.m31 + B.m31, A.m32 + B.m32, A.m33 + B.m33, A.m34 + B.m34,
		A.m41 + B.m41, A.m42 + B.m42, A.m43 + B.m43, A.m44 + B.m44
	);
}


/*
** Return negative of matrix
*/
static inline struct mat4
mat4_neg(struct mat4 M) {
	return mat4_make(
		-M.m11, -M.m12, -M.m13, -M.m14,
		-M.m21, -M.m22, -M.m23, -M.m24,
		-M.m31, -M.m32, -M.m33, -M.m34,
		-M.m41, -M.m42, -M.m43, -M.m44
	);
}


/*
** Component-wise matrix subtraction (A - B)
*/
static inline struct mat4
mat4_sub(struct mat4 A, struct mat4 B) {
	return mat4_make(
		A.m11 - B.m11, A.m12 - B.m12, A.m13 - B.m13, A.m14 - B.m14,
		A.m21 - B.m21, A.m22 - B.m22, A.m23 - B.m23, A.m24 - B.m24,
		A.m31 - B.m31, A.m32 - B.m32, A.m33 - B.m33, A.m34 - B.m34,
		A.m41 - B.m41, A.m42 - B.m42, A.m43 - B.m43, A.m44 - B.m44
	);
}


/*
** Multiply a scalar s by a matrix M
*/
static inline struct mat4
mat4_scale(float s, struct mat4 M) {
	return mat4_make(
		s * M.m11, s * M.m12, s * M.m13, s * M.m14,
		s * M.m21, s * M.m22, s * M.m23, s * M.m24,
		s * M.m31, s * M.m32, s * M.m33, s * M.m34,
		s * M.m41, s * M.m42, s * M.m43, s * M.m44
	);
}


/*
** Returns rotation matrix with angle-axis formula
*/
static inline struct mat4
mat4_rotate(float angleInDegrees, struct vec3 v) {
	float angleInRadians = angleInDegrees * 3.141592653f / 180.0f;
	float c = cosf(angleInRadians);
	float s = sinf(angleInRadians);
	v = normalize(v);

	return mat4_make(
		v.x * v.x * (1 - c) + c, v.x * v.y * (1 - c) - v.z * s, v.x * v.z * (1 - c) + v.y * s, 0.0f,
		v.y * v.x * (1 - c) + v.z * s, v.y * v.y * (1 - c) + c, v.y * v.z * (1 - c) - v.x * s, 0.0f,
		v.x * v.z * (1 - c) - v.y * s, v.y * v.z * (1 - c) + v.x * s, v.z * v.z * (1 - c) + c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}


/*
** Return translation matrix
*/
static inline struct mat4
mat4_translate(struct vec3 T) {
	return mat4_make(
		1.0f, 0.0f, 0.0f, T.x,
		0.0f, 1.0f, 0.0f, T.y,
		0.0f, 0.0f, 1.0f, T.z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}


/*
** Return scaling matrix
*/
static inline struct mat4
mat4_scaling(struct vec3 S) {
	return mat4_make(
		S.x, 0.0f, 0.0f, 0.0f,
		0.0f, S.y, 0.0f, 0.0f,
		0.0f, 0.0f, S.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}


/*
** Returns ortho projection matrix
*/
static inline struct mat4
mat4_ortho(float left, float right, float bottom, float top, float nearZ, float farZ) {
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(farZ + nearZ) / (farZ - nearZ);

	return mat4_make(
		2.0f / (right - left), 0.0f, 0.0f, tx,
		0.0f, 2.0f / (top - bottom), 0.0f, ty,
		0.0f, 0.0f, -2.0f / (farZ - nearZ), tz,
		0.0f, 0.0f, 0.0f, 1.0f);
}


/*
** Returns 2D ortho projection matrix
*/
static inline struct mat4
mat4_ortho2D(float left, float right, float bottom, float top) {
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);

	return mat4_make(
		2.0f / (right - left), 0.0f, 0.0f, tx,
		0.0f, 2.0f / (top - bottom), 0.0f, ty,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}


static inline struct mat4
mat4_frustum(float left, float right, float bottom, float top, float nearZ, float farZ) {
	float A = (right + left) / (right - left);
	float B = (top + bottom) / (top - bottom);
	float C = -(farZ + nearZ) / (farZ - nearZ);
	float D = -2.0f * farZ * nearZ / (farZ - nearZ);

	return mat4_frustum(
		2.0f * nearZ / (right - left), 0.0f, A, 0.0f,
		0.0f, 2.0f * nearZ / (top - bottom), B, 0.0f,
		0.0f, 0.0f, C, D,
		0.0f, 0.0f, -1.0f, 0.0f);
}


/*
** Returns perspective projection matrix with vertical field of view
*/
static inline struct mat4
mat4_perspective(float angleInDegrees, float aspect, float nearZ, float farZ) {
	float f = (1.0f / tanf(FX_F32_DEGREES_TO_RADIANS * 0.5f * angleInDegrees));

	return mat4_make(
		f / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0,
		0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), 2.0f * farZ * nearZ / (nearZ - farZ),
		0.0f, 0.0f, -1.0f, 0.0f);
}


#ifdef __cplusplus
// define unary and binary operators
#endif


#endif

