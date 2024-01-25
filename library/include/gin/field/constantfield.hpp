#pragma once

#include <gin/field/field.hpp>

namespace Gin::Field {

    template<typename T>
    class ConstantField : public BaseField {
    public:
        ConstantField() = default;
        ConstantField(T v) : v{ v } {};

        Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) final {
            return (Math::Scalar)v;
        };

        Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector2{ (Math::Scalar)v, (Math::Scalar)v };
        };

        Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector3{ (Math::Scalar)v, (Math::Scalar)v, (Math::Scalar)v };
        };

        Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector4{ (Math::Scalar)v, (Math::Scalar)v, (Math::Scalar)v, (Math::Scalar)v };
        };

        xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) final {
            return xsimd::batch<Math::Scalar>( (Math::Scalar)v );
        };

        VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> b = GetScalarBatch(0, 0, 0);
            VectorBatch<Math::Scalar, 2> r{};
            r.c[0] = b;
            r.c[1] = b;
            return r;
        };

        VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> b = GetScalarBatch(0, 0, 0);
            VectorBatch<Math::Scalar, 3> r{};
            r.c[0] = b;
            r.c[1] = b;
            r.c[2] = b;
            return r;
        };

        VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> b = GetScalarBatch(0, 0, 0);
            VectorBatch<Math::Scalar, 4> r{};
            r.c[0] = b;
            r.c[1] = b;
            r.c[2] = b;
            r.c[3] = b;
            return r;
        };


        uint32_t GetComponentCount() final {
            return 1;
        };

        T& Get() { return v; };

    private:
        T v{};
    };

    template<>
    class ConstantField<Math::Vector2> : public BaseField {
    public:
        ConstantField() = default;
        ConstantField(Math::Vector2 v) : v{ v } {};

        Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) final {
            return v.x;
        };

        Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) final {
            return v;
        };

        Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector3{ v.x, v.y, v.y };
        };

        Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector4{ v.x, v.y, v.y, v.y };
        };

        xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) final {
            return xsimd::batch<Math::Scalar>( (Math::Scalar)v.x );
        };

        VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) final {
            VectorBatch<Math::Scalar, 2> r{};
            r.c[0] = v.x;
            r.c[1] = v.y;
            return r;
        };

        VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) final {
            VectorBatch<Math::Scalar, 3> r{};
            r.c[0] = v.x;
            r.c[1] = v.y;
            r.c[2] = v.y;
            return r;
        };

        VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> b = GetScalarBatch(0, 0, 0);
            VectorBatch<Math::Scalar, 4> r{};
            r.c[0] = v.x;
            r.c[1] = v.y;
            r.c[2] = v.y;
            r.c[3] = v.y;
            return r;
        };

        uint32_t GetComponentCount() final {
            return 2;
        };

        Math::Vector2& Get() { return v; };

    private:
        Math::Vector2 v{};
    };


    template<>
    class ConstantField<Math::Vector3> : public BaseField {
    public:
        ConstantField() = default;
        ConstantField(Math::Vector3 v) : v{ v } {};

        Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) final {
            return v.x;
        };

        Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector2{ v.x, v.y };
        };

        Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) final {
            return v;
        };

        Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector4(v.x, v.y, v.y, v.y);
        };

        xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) final {
            return xsimd::batch<Math::Scalar>( (Math::Scalar)v.x );
        };

        VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) final {
            VectorBatch<Math::Scalar, 2> r{};
            r.c[0] = v.x;
            r.c[1] = v.y;
            return r;
        };

        VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) final {
            VectorBatch<Math::Scalar, 3> r{};
            r.c[0] = v.x;
            r.c[1] = v.y;
            r.c[2] = v.z;
            return r;
        };

        VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> b = GetScalarBatch(0, 0, 0);
            VectorBatch<Math::Scalar, 4> r{};
            r.c[0] = v.x;
            r.c[1] = v.y;
            r.c[2] = v.z;
            r.c[3] = v.z;
            return r;
        };


        uint32_t GetComponentCount() final {
            return 3;
        };

        Math::Vector3& Get() { return v; };

    private:
        Math::Vector3 v{};
    };


    template<>
    class ConstantField<Math::Vector4> : public BaseField {
    public:
        ConstantField() = default;
        ConstantField(Math::Vector4 v) : v{ v } {};

        Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) final {
            return v.x;
        };

        Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector2{ v.x, v.y };
        };

        Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector3{ v.x, v.y, v.z };
        };

        Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) final {
            return v;
        };

        xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) final {
            return xsimd::batch<Math::Scalar>( (Math::Scalar)v.x );
        };

        VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) final {
            VectorBatch<Math::Scalar, 2> r{};
            r.c[0] = v.x;
            r.c[1] = v.y;
            return r;
        };

        VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) final {
            VectorBatch<Math::Scalar, 3> r{};
            r.c[0] = v.x;
            r.c[1] = v.y;
            r.c[2] = v.z;
            return r;
        };

        VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> b = GetScalarBatch(0, 0, 0);
            VectorBatch<Math::Scalar, 4> r{};
            r.c[0] = v.x;
            r.c[1] = v.y;
            r.c[2] = v.z;
            r.c[3] = v.w;
            return r;
        };


        uint32_t GetComponentCount() final {
            return 1;
        };

        Math::Vector4& Get() { return v; };

    private:
        Math::Vector4 v{};
    };

}