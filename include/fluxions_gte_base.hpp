#ifndef FLUXIONS_GTE_BASE_HPP
#define FLUXIONS_GTE_BASE_HPP

#include <cmath>

struct FxColor4f {
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 0.0f;
	FxColor4f(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) :r(r), g(g), b(b), a(a) {}
};

struct FxVector3f {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

inline float length(FxVector3f v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline FxVector3f normalize(FxVector3f v) {
	float mag = length(v);
	if (mag == 0.0f) return { 0.0f, 0.0f, 0.0f };
	mag = 1.0f / mag;
	return { v.x * mag,v.y * mag,v.z * mag };
}

inline float dot(FxVector3f a, FxVector3f b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline FxVector3f cross(FxVector3f a, FxVector3f b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

struct FxMatrix4f {
	float m11 = 1.0f, m21 = 0.0f, m31 = 0.0f, m41 = 0.0f;
	float m12 = 0.0f, m22 = 1.0f, m32 = 0.0f, m42 = 0.0f;
	float m13 = 0.0f, m23 = 0.0f, m33 = 1.0f, m43 = 0.0f;
	float m14 = 0.0f, m24 = 0.0f, m34 = 0.0f, m44 = 1.0f;

	FxMatrix4f() {}

	FxMatrix4f(float a11, float a12, float a13, float a14,
			   float a21, float a22, float a23, float a24,
			   float a31, float a32, float a33, float a34,
			   float a41, float a42, float a43, float a44) :
		m11(a11), m21(a21), m31(a31), m41(a41),
		m12(a12), m22(a22), m32(a32), m42(a42),
		m13(a13), m23(a23), m33(a33), m43(a43),
		m14(a14), m24(a24), m34(a34), m44(a44) {}

	void loadIdentity() {
		*this = FxMatrix4f();
	}

	FxMatrix4f operator*(const FxMatrix4f& m2) {
		return FxMatrix4f(
			(m14 * m2.m41 + m13 * m2.m31 + m12 * m2.m21 + m11 * m2.m11),
			(m14 * m2.m42 + m13 * m2.m32 + m12 * m2.m22 + m11 * m2.m12),
			(m14 * m2.m43 + m13 * m2.m33 + m12 * m2.m23 + m11 * m2.m13),
			(m14 * m2.m44 + m13 * m2.m34 + m12 * m2.m24 + m11 * m2.m14),

			(m24 * m2.m41 + m23 * m2.m31 + m22 * m2.m21 + m21 * m2.m11),
			(m24 * m2.m42 + m23 * m2.m32 + m22 * m2.m22 + m21 * m2.m12),
			(m24 * m2.m43 + m23 * m2.m33 + m22 * m2.m23 + m21 * m2.m13),
			(m24 * m2.m44 + m23 * m2.m34 + m22 * m2.m24 + m21 * m2.m14),

			(m34 * m2.m41 + m33 * m2.m31 + m32 * m2.m21 + m31 * m2.m11),
			(m34 * m2.m42 + m33 * m2.m32 + m32 * m2.m22 + m31 * m2.m12),
			(m34 * m2.m43 + m33 * m2.m33 + m32 * m2.m23 + m31 * m2.m13),
			(m34 * m2.m44 + m33 * m2.m34 + m32 * m2.m24 + m31 * m2.m14),

			(m44 * m2.m41 + m43 * m2.m31 + m42 * m2.m21 + m41 * m2.m11),
			(m44 * m2.m42 + m43 * m2.m32 + m42 * m2.m22 + m41 * m2.m12),
			(m44 * m2.m43 + m43 * m2.m33 + m42 * m2.m23 + m41 * m2.m13),
			(m44 * m2.m44 + m43 * m2.m34 + m42 * m2.m24 + m41 * m2.m14));
	}

	FxMatrix4f& rotate(float angleInDegrees, FxVector3f v) {
		float angleInRadians = angleInDegrees * 180.0f / 3.141592653f;
		float c = cos(angleInRadians);
		float s = sin(angleInRadians);
		float invLength = (float)(1.0 / sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
		v.x *= invLength;
		v.y *= invLength;
		v.z *= invLength;

		*this = *this * FxMatrix4f(
			v.x * v.x * (1 - c) + c, v.x * v.y * (1 - c) - v.z * s, v.x * v.z * (1 - c) + v.y * s, 0.0,
			v.y * v.x * (1 - c) + v.z * s, v.y * v.y * (1 - c) + c, v.y * v.z * (1 - c) - v.x * s, 0.0,
			v.x * v.z * (1 - c) - v.y * s, v.y * v.z * (1 - c) + v.x * s, v.z * v.z * (1 - c) + c, 0.0,
			0.0f, 0.0f, 0.0f, 1.0f);

		return *this;
	}

	FxMatrix4f& translate(FxVector3f v) {
		*this = *this * FxMatrix4f(
			1.0f, 0.0f, 0.0f, v.x,
			0.0f, 1.0f, 0.0f, v.y,
			0.0f, 0.0f, 1.0f, v.z,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return *this;
	}

	FxMatrix4f& scale(FxVector3f v) {
		*this = *this * FxMatrix4f(
			v.x, 0.0f, 0.0f, 0.0f,
			0.0f, v.y, 0.0f, 0.0f,
			0.0f, 0.0f, v.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		return *this;
	}

	FxMatrix4f& ortho(float left, float right, float bottom, float top, float nearZ, float farZ) {
		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);
		float tz = -(farZ + nearZ) / (farZ - nearZ);

		*this = *this * FxMatrix4f(
			2.0f / (right - left), 0.0f, 0.0f, tx,
			0.0f, 2.0f / (top - bottom), 0.0f, ty,
			0.0f, 0.0f, -2.0f / (farZ - nearZ), tz,
			0.0f, 0.0f, 0.0f, 1.0f);

		return *this;
	}

	FxMatrix4f& ortho2D(float left, float right, float bottom, float top) {
		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);

		*this = *this * FxMatrix4f(
			2.0f / (right - left), 0.0f, 0.0f, tx,
			0.0f, 2.0f / (top - bottom), 0.0f, ty,
			0.0f, 0.0f, -1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return *this;
	}

	FxMatrix4f& frustum(float left, float right, float bottom, float top, float nearZ, float farZ) {
		float A = (right + left) / (right - left);
		float B = (top + bottom) / (top - bottom);
		float C = -(farZ + nearZ) / (farZ - nearZ);
		float D = -2.0f * farZ * nearZ / (farZ - nearZ);

		*this = *this * FxMatrix4f(
			2.0f * nearZ / (right - left), 0.0f, A, 0.0f,
			0.0f, 2.0f * nearZ / (top - bottom), B, 0.0f,
			0.0f, 0.0f, C, D,
			0.0f, 0.0f, -1.0f, 0.0f);

		return *this;
	}

	FxMatrix4f& perspective(float angleInDegrees, float aspect, float nearZ, float farZ) {
		float f = (1.0f / std::tan(180.0f / 3.141592653f * 0.5f * angleInDegrees));

		*this = *this * FxMatrix4f(
			f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, f, 0.0f, 0,
			0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), 2.0f * farZ * nearZ / (nearZ - farZ),
			0.0f, 0.0f, -1.0f, 0.0f);

		return *this;
	}
};

#endif