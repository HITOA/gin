#pragma once

#include <gin/math/type.hpp>

namespace Gin::Math {

    inline Vector3 Cross(const Vector3& rhs, const Vector3& lhs) {
        return Vector3{
            rhs.y * lhs.z - rhs.z * lhs.y,
            -(rhs.x * lhs.z - rhs.z * lhs.x),
            rhs.x * lhs.y - rhs.y * lhs.x
        };
    }

    inline float Dot(const Vector3& rhs, const Vector3& lhs) {
        return rhs.x * lhs.x + rhs.y * lhs.y + rhs.z * lhs.z;
    }

    inline float Length(const Vector3& rhs) {
        return std::sqrt(Dot(rhs, rhs));
    }

    inline Vector3 Normalize(Vector3 rhs) {
        return rhs / Length(rhs);
    }

    /*
	template<typename T, typename U, size_t Dim>
	inline Eigen::Vector<U, Dim> Ceil(Eigen::Vector<T, Dim>& vec) {
		Eigen::Vector<U, Dim> r{};
		for (size_t i = 0; i < Dim; ++i)
			r[i] = (U)std::ceil(vec[i]);
		return r;
	}

	template<typename T, typename U, size_t Dim>
	inline Eigen::Vector<U, Dim> Floor(Eigen::Vector<T, Dim>& vec) {
		Eigen::Vector<U, Dim> r{};
		for (size_t i = 0; i < Dim; ++i)
			r[i] = (U)std::floor(vec[i]);
		return r;
	}

	template<typename T, typename U, size_t Dim>
	inline Eigen::Vector<U, Dim> Round(Eigen::Vector<T, Dim>& vec) {
		Eigen::Vector<U, Dim> r{};
		for (size_t i = 0; i < Dim; ++i)
			r[i] = (U)std::round(vec[i]);
		return r;
	}

	template<typename T>
	inline T Min(const T& a, const T& b) {
		return a <= b ? a : b;
	}

	template<typename T>
	inline T Max(const T& a, const T& b) {
		return a >= b ? a : b;
	}

	template<typename T>
	inline T Clamp(const T& a, const T& min, const T& max) {
		return Max<T>(Min<T>(a, max), min);
	}

	inline Eigen::Vector2<float> SMin(float a, float b, float k) {
		float h = Max<float>(k - abs(a - b), 0.0f) / k;
		float m = h * h * 0.5f;
		float s = m * k * (1.0f / 2.0f);
		return a < b ? Eigen::Vector2<float>{ a - s, m } : Eigen::Vector2<float>{ b - s, 1.0f - m };
	}

	inline Eigen::Vector2<float> SMax(float a, float b, float k) {
		float h = Max<float>(k - abs(a - b), 0.0f) / k;
		float m = h * h * h * 0.5f;
		float s = m * k * (1.0f / 3.0f);
		return a > b ? Eigen::Vector2<float>{ a - s, m } : Eigen::Vector2<float>{ b - s, 1.0f - m };
	}*/
}