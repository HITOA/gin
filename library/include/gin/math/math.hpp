#pragma once

#include <gin/math/type.hpp>

namespace Gin::Math {

    template<typename T>
    inline BaseVector3<T> Cross(const BaseVector3<T>& rhs, const BaseVector3<T>& lhs) {
        return BaseVector3<T>{
            rhs.y * lhs.z - rhs.z * lhs.y,
            -(rhs.x * lhs.z - rhs.z * lhs.x),
            rhs.x * lhs.y - rhs.y * lhs.x
        };
    }

    template<typename T>
    inline float Dot(const BaseVector3<T>& rhs, const BaseVector3<T>& lhs) {
        return rhs.x * lhs.x + rhs.y * lhs.y + rhs.z * lhs.z;
    }

    template<typename T>
    inline float Length(const BaseVector3<T>& rhs) {
        return std::sqrt(Dot(rhs, rhs));
    }

    template<typename T>
    inline BaseVector3<T> Normalize(BaseVector3<T> rhs) {
        return rhs / Length(rhs);
    }

    template<typename T>
    inline BaseVector2<T> Ceil(BaseVector2<T> rhs) {
        rhs.x = std::ceil(rhs.x);
        rhs.y = std::ceil(rhs.y);
        return rhs;
    }

    template<typename T>
    inline BaseVector3<T> Ceil(BaseVector3<T> rhs) {
        rhs.x = std::ceil(rhs.x);
        rhs.y = std::ceil(rhs.y);
        rhs.z = std::ceil(rhs.z);
        return rhs;
    }

    template<typename T>
    inline BaseVector4<T> Ceil(BaseVector4<T> rhs) {
        rhs.x = std::ceil(rhs.x);
        rhs.y = std::ceil(rhs.y);
        rhs.z = std::ceil(rhs.z);
        rhs.w = std::ceil(rhs.w);
        return rhs;
    }

    template<typename T>
    inline BaseVector2<T> Floor(BaseVector2<T> rhs) {
        rhs.x = std::floor(rhs.x);
        rhs.y = std::floor(rhs.y);
        return rhs;
    }

    template<typename T>
    inline BaseVector3<T> Floor(BaseVector3<T> rhs) {
        rhs.x = std::floor(rhs.x);
        rhs.y = std::floor(rhs.y);
        rhs.z = std::floor(rhs.z);
        return rhs;
    }

    template<typename T>
    inline BaseVector4<T> Floor(BaseVector4<T> rhs) {
        rhs.x = std::floor(rhs.x);
        rhs.y = std::floor(rhs.y);
        rhs.z = std::floor(rhs.z);
        rhs.w = std::floor(rhs.w);
        return rhs;
    }

    template<typename T>
    inline BaseVector2<T> Round(BaseVector2<T> rhs) {
        rhs.x = std::round(rhs.x);
        rhs.y = std::round(rhs.y);
        return rhs;
    }

    template<typename T>
    inline BaseVector3<T> Round(BaseVector3<T> rhs) {
        rhs.x = std::round(rhs.x);
        rhs.y = std::round(rhs.y);
        rhs.z = std::round(rhs.z);
        return rhs;
    }

    template<typename T>
    inline BaseVector4<T> Round(BaseVector4<T> rhs) {
        rhs.x = std::round(rhs.x);
        rhs.y = std::round(rhs.y);
        rhs.z = std::round(rhs.z);
        rhs.w = std::round(rhs.w);
        return rhs;
    }

	/*template<typename T>
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