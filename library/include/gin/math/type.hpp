#pragma once

namespace Gin::Math {

    typedef float Scalar;

    template<typename T>
    struct BaseVector2 {
        T x{}, y{};

        BaseVector2() = default;
        explicit BaseVector2(T v) : x{ v }, y{ v } {};
        BaseVector2(T x, T y) : x{ x }, y{ y } {};

        inline BaseVector2<T>& operator+=(T lhs) {
            x += lhs;
            y += lhs;
            return *this;
        }
        inline BaseVector2<T>& operator-=(T lhs) {
            x -= lhs;
            y -= lhs;
            return *this;
        }
        inline BaseVector2<T>& operator*=(T lhs) {
            x *= lhs;
            y *= lhs;
            return *this;
        }
        inline BaseVector2<T>& operator/=(T lhs) {
            x /= lhs;
            y /= lhs;
            return *this;
        }

        inline BaseVector2<T>& operator+=(const BaseVector2<T>& lhs) {
            x += lhs.x;
            y += lhs.y;
            return *this;
        }
        inline BaseVector2<T>& operator-=(const BaseVector2<T>& lhs) {
            x -= lhs.x;
            y -= lhs.y;
            return *this;
        }
        inline BaseVector2<T>& operator*=(const BaseVector2<T>& lhs) {
            x *= lhs.x;
            y *= lhs.y;
            return *this;
        }
        inline BaseVector2<T>& operator/=(const BaseVector2<T>& lhs) {
            x /= lhs.x;
            y /= lhs.y;
            return *this;
        }

        friend inline BaseVector2<T> operator+(BaseVector2<T> rhs, T lhs) {
            rhs += lhs;
            return rhs;
        }
        friend inline BaseVector2<T> operator-(BaseVector2<T> rhs, T lhs) {
            rhs -= lhs;
            return rhs;
        }
        friend inline BaseVector2<T> operator*(BaseVector2<T> rhs, T lhs) {
            rhs *= lhs;
            return rhs;
        }
        friend inline BaseVector2<T> operator/(BaseVector2<T> rhs, T lhs) {
            rhs /= lhs;
            return rhs;
        }

        friend inline BaseVector2<T> operator+(BaseVector2<T> rhs, const BaseVector2<T>& lhs) {
            rhs += lhs;
            return rhs;
        }
        friend inline BaseVector2<T> operator-(BaseVector2<T> rhs, const BaseVector2<T>& lhs) {
            rhs -= lhs;
            return rhs;
        }
        friend inline BaseVector2<T> operator*(BaseVector2<T> rhs, const BaseVector2<T>& lhs) {
            rhs *= lhs;
            return rhs;
        }
        friend inline BaseVector2<T> operator/(BaseVector2<T> rhs, const BaseVector2<T>& lhs) {
            rhs /= lhs;
            return rhs;
        }
    };

    template<typename T>
    struct BaseVector3 {
        T x{}, y{}, z{};

        BaseVector3() = default;
        explicit BaseVector3(T v) : x{ v }, y{ v }, z{ v } {};
        BaseVector3(T x, T y, T z) : x{ x }, y{ y }, z{ z } {};

        inline BaseVector3<T>& operator+=(T lhs) {
            x += lhs;
            y += lhs;
            z += lhs;
            return *this;
        }
        inline BaseVector3<T>& operator-=(T lhs) {
            x -= lhs;
            y -= lhs;
            z -= lhs;
            return *this;
        }
        inline BaseVector3<T>& operator*=(T lhs) {
            x *= lhs;
            y *= lhs;
            z *= lhs;
            return *this;
        }
        inline BaseVector3<T>& operator/=(T lhs) {
            x /= lhs;
            y /= lhs;
            z /= lhs;
            return *this;
        }

        inline BaseVector3<T>& operator+=(const BaseVector3<T>& lhs) {
            x += lhs.x;
            y += lhs.y;
            z += lhs.z;
            return *this;
        }
        inline BaseVector3<T>& operator-=(const BaseVector3<T>& lhs) {
            x -= lhs.x;
            y -= lhs.y;
            z -= lhs.z;
            return *this;
        }
        inline BaseVector3<T>& operator*=(const BaseVector3<T>& lhs) {
            x *= lhs.x;
            y *= lhs.y;
            z *= lhs.z;
            return *this;
        }
        inline BaseVector3<T>& operator/=(const BaseVector3<T>& lhs) {
            x /= lhs.x;
            y /= lhs.y;
            z /= lhs.z;
            return *this;
        }

        friend inline BaseVector3<T> operator+(BaseVector3<T> rhs, T lhs) {
            rhs += lhs;
            return rhs;
        }
        friend inline BaseVector3<T> operator-(BaseVector3<T> rhs, T lhs) {
            rhs -= lhs;
            return rhs;
        }
        friend inline BaseVector3<T> operator*(BaseVector3<T> rhs, T lhs) {
            rhs *= lhs;
            return rhs;
        }
        friend inline BaseVector3<T> operator/(BaseVector3<T> rhs, T lhs) {
            rhs /= lhs;
            return rhs;
        }

        friend inline BaseVector3<T> operator+(BaseVector3<T> rhs, const BaseVector3<T>& lhs) {
            rhs += lhs;
            return rhs;
        }
        friend inline BaseVector3<T> operator-(BaseVector3<T> rhs, const BaseVector3<T>& lhs) {
            rhs -= lhs;
            return rhs;
        }
        friend inline BaseVector3<T> operator*(float rhs, const BaseVector3<T>& lhs) {
            rhs *= lhs;
            return rhs;
        }
        friend inline BaseVector3<T> operator/(BaseVector3<T> rhs, const BaseVector3<T>& lhs) {
            rhs /= lhs;
            return rhs;
        }
    };

    template<typename T>
    struct BaseVector4 {
        T x{}, y{}, z{}, w{};

        BaseVector4() = default;
        explicit BaseVector4(T v) : x{ v }, y{ v }, z{ v }, w{ v } {};
        BaseVector4(T x, T y, T z, T w) : x{ x }, y{ y }, z{ z }, w{ w } {};

        inline BaseVector4<T>& operator+=(T lhs) {
            x += lhs;
            y += lhs;
            z += lhs;
            w += lhs;
            return *this;
        }
        inline BaseVector4<T>& operator-=(T lhs) {
            x -= lhs;
            y -= lhs;
            z -= lhs;
            w -= lhs;
            return *this;
        }
        inline BaseVector4<T>& operator*=(T lhs) {
            x *= lhs;
            y *= lhs;
            z *= lhs;
            w *= lhs;
            return *this;
        }
        inline BaseVector4<T>& operator/=(T lhs) {
            x /= lhs;
            y /= lhs;
            z /= lhs;
            w /= lhs;
            return *this;
        }

        inline BaseVector4<T>& operator+=(const BaseVector4<T>& lhs) {
            x += lhs.x;
            y += lhs.y;
            z += lhs.z;
            w += lhs.w;
            return *this;
        }
        inline BaseVector4<T>& operator-=(const BaseVector4<T>& lhs) {
            x -= lhs.x;
            y -= lhs.y;
            z -= lhs.z;
            w -= lhs.w;
            return *this;
        }
        inline BaseVector4<T>& operator*=(const BaseVector4<T>& lhs) {
            x *= lhs.x;
            y *= lhs.y;
            z *= lhs.z;
            w *= lhs.w;
            return *this;
        }
        inline BaseVector4<T>& operator/=(const BaseVector4<T>& lhs) {
            x /= lhs.x;
            y /= lhs.y;
            z /= lhs.z;
            w /= lhs.w;
            return *this;
        }

        friend inline BaseVector4<T> operator+(BaseVector4<T> rhs, T lhs) {
            rhs += lhs;
            return rhs;
        }
        friend inline BaseVector4<T> operator-(BaseVector4<T> rhs, T lhs) {
            rhs -= lhs;
            return rhs;
        }
        friend inline BaseVector4<T> operator*(BaseVector4<T> rhs, T lhs) {
            rhs *= lhs;
            return rhs;
        }
        friend inline BaseVector4<T> operator/(BaseVector4<T> rhs, T lhs) {
            rhs /= lhs;
            return rhs;
        }

        friend inline BaseVector4<T> operator+(BaseVector4<T> rhs, const BaseVector4<T>& lhs) {
            rhs += lhs;
            return rhs;
        }
        friend inline BaseVector4<T> operator-(BaseVector4<T> rhs, const BaseVector4<T>& lhs) {
            rhs -= lhs;
            return rhs;
        }
        friend inline BaseVector4<T> operator*(BaseVector4<T> rhs, const BaseVector4<T>& lhs) {
            rhs *= lhs;
            return rhs;
        }
        friend inline BaseVector4<T> operator/(BaseVector4<T> rhs, const BaseVector4<T>& lhs) {
            rhs /= lhs;
            return rhs;
        }
    };

    typedef BaseVector2<Scalar> Vector2;
    typedef BaseVector3<Scalar> Vector3;
    typedef BaseVector4<Scalar> Vector4;
    typedef BaseVector2<int> Vector2Int;
    typedef BaseVector3<int> Vector3Int;
    typedef BaseVector4<int> Vector4Int;

    struct Bounds {
        Vector3 origin{};
        Vector3 extent{};
    };

    struct Sphere {
        Vector3 origin{};
        float radius{};
    };

}