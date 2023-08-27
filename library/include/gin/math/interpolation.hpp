#pragma once

namespace Gin::Math {

	template<typename T>
	inline T Linear(const T& v0, const T& v1, float t) {
		return (1.0f - t) * v0 + t * v1;
	}

	template<typename T>
	inline T Bilinear(const T& v00, const T& v10, const T& v01, const T& v11, float tx, float ty) {
		return Linear<T>(Linear<T>(v00, v10, tx), Linear<T>(v01, v11, tx), ty);
	}

	template<typename T>
	inline T Trilinear(const T& v000, const T& v100, const T& v010, const T& v110, const T& v001, const T& v101, const T& v011, const T& v111, float tx, float ty, float tz) {
		return Linear<T>(Bilinear<T>(v000, v100, v010, v110, tx, ty), Bilinear<T>(v001, v101, v011, v111, tx, ty), tz);
	}

	template<typename T>
	inline T Smoothstep(T v, T edge0 = T{ 0.0f }, T edge1 = T{ 1.0f }) {
		v = (v - edge0) / (edge1 - edge0);
		v = v < 0.0f ? 0.0f : v > 1.0f ? 1.0f : v;
		return v * v * (3.0f - 2.0f * v);
	}

	template<typename T>
	inline T InverseLerp(T a, T b, float t) {
		return (t - a) / (b - a);
	}

}
