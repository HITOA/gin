#pragma once

#include <cstdint>
#include <cstddef>
#include <gin/math/type.hpp>
#include <xsimd/xsimd.hpp>
#include <gin/profiler/profiler.hpp>

namespace Gin::Field {

    template<typename T, size_t Dim>
    struct VectorBatch {
        xsimd::batch<T> c[Dim];
    };

    class BaseField {
    public:
        virtual ~BaseField() = default;

        virtual Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) = 0;
        virtual Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) = 0;
        virtual Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) = 0;
        virtual Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) = 0;

        virtual xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) = 0;
        virtual VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) = 0;
        virtual VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) = 0;
        virtual VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) = 0;

        virtual uint32_t GetComponentCount() = 0;
    };

    template<typename T>
    class ScalarField : public BaseField {
    public:
        ScalarField() = delete;
        ScalarField(uint32_t width, uint32_t height, uint32_t depth) :
                width{ width }, height{ height }, depth{ depth } {
            vecWidth = width + width % xsimd::simd_type<Math::Scalar>::size;
            data = (T*)xsimd::aligned_malloc(vecWidth * height * depth * sizeof(T), xsimd::default_arch::alignment());
            Profiler::RecordAllocation(vecWidth * height * depth * sizeof(T), data);
        }
        ~ScalarField() final {
            if (data) {
                Profiler::RecordDeallocation(data);
                xsimd::aligned_free(data);
            }
        }

        Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) final {
            return (Math::Scalar)data[x + y * vecWidth + z * vecWidth * height];
        }

        Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector2{ (Math::Scalar)data[x + y * vecWidth + z * vecWidth * height] };
        }

        Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector3{ (Math::Scalar)data[x + y * vecWidth + z * vecWidth * height] };
        }

        Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) final {
            return Math::Vector4{ (Math::Scalar)data[x + y * vecWidth + z * vecWidth * height] };
        }

        xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> r = xsimd::load_aligned((Math::Scalar*)&data[x + y * vecWidth + z * vecWidth * height]);
            return r;
        };

        VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> b = GetScalar(x, y, z);
            VectorBatch<Math::Scalar, 2> r{};
            for (auto & i : r.c)
                i = b;
            return r;
        };

        VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> b = GetScalar(x, y, z);
            VectorBatch<Math::Scalar, 3> r{};
            for (auto & i : r.c)
                i = b;
            return r;
        };

        VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) final {
            xsimd::batch<Math::Scalar> b = GetScalar(x, y, z);
            VectorBatch<Math::Scalar, 4> r{};
            for (auto & i : r.c)
                i = b;
            return r;
        };

        uint32_t GetComponentCount() final {
            return 1;
        }

        uint32_t GetWidth() { return width; };
        uint32_t GetHeight() { return height; };
        uint32_t GetDepth() { return depth; };
        uint32_t GetVecWidth() { return vecWidth; };

        inline T& operator[](size_t idx) {
            return data[idx];
        }

    private:
        uint32_t width{}, height{}, depth{};
        uint32_t vecWidth{};
        T* data{ nullptr };
    };

    class VectorizedVector2Field : public BaseField {
    public:
        struct VectorVector2 {
            Math::Scalar x[xsimd::simd_type<Math::Scalar>::size];
            Math::Scalar y[xsimd::simd_type<Math::Scalar>::size];
        };
    public:
        VectorizedVector2Field() = delete;
        VectorizedVector2Field(uint32_t width, uint32_t height, uint32_t depth) :
                width{ width }, height{ height }, depth{ depth } {
            vecWidth = width + width % xsimd::simd_type<Math::Scalar>::size;
            data = (Math::Vector2*)xsimd::aligned_malloc(vecWidth * height * depth * sizeof(Math::Vector2),
                                                         xsimd::default_arch::alignment());
            Profiler::RecordAllocation(vecWidth * height * depth * sizeof(Math::Vector2), data);
        }
        ~VectorizedVector2Field() final {
            if (data) {
                Profiler::RecordDeallocation(data);
                xsimd::aligned_free(data);
            }
        }

        Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv2Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector2& vv2 = GetVectorVector2(vv2Idx);
            return vv2.x[cIdx];
        }

        Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv2Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector2& vv2 = GetVectorVector2(vv2Idx);
            return Math::Vector2{ vv2.x[cIdx], vv2.y[cIdx] };
        }

        Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv2Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector2& vv2 = GetVectorVector2(vv2Idx);
            return Math::Vector3{ vv2.x[cIdx], vv2.y[cIdx], vv2.y[cIdx] };
        }

        Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv2Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector2& vv2 = GetVectorVector2(vv2Idx);
            return Math::Vector4{ vv2.x[cIdx], vv2.y[cIdx], vv2.y[cIdx], vv2.y[cIdx] };
        }

        xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv2Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector2& vv2 = GetVectorVector2(vv2Idx);
            xsimd::batch<Math::Scalar> r = xsimd::load_aligned(vv2.x);
            return r;
        };

        VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv2Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector2& vv2 = GetVectorVector2(vv2Idx);
            VectorBatch<Math::Scalar, 2> r{};
            r.c[0] = xsimd::load_aligned(vv2.x);
            r.c[1] = xsimd::load_aligned(vv2.y);
            return r;
        };

        VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv2Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector2& vv2 = GetVectorVector2(vv2Idx);
            VectorBatch<Math::Scalar, 3> r{};
            r.c[0] = xsimd::load_aligned(vv2.x);
            r.c[1] = xsimd::load_aligned(vv2.y);
            r.c[2] = xsimd::load_aligned(vv2.y);
            return r;
        };

        VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) final {

            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv2Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector2& vv2 = GetVectorVector2(vv2Idx);
            VectorBatch<Math::Scalar, 4> r{};
            r.c[0] = xsimd::load_aligned(vv2.x);
            r.c[1] = xsimd::load_aligned(vv2.y);
            r.c[2] = xsimd::load_aligned(vv2.y);
            r.c[3] = xsimd::load_aligned(vv2.y);
            return r;
        };

        uint32_t GetComponentCount() final {
            return 3;
        }

        uint32_t GetWidth() { return width; };
        uint32_t GetHeight() { return height; };
        uint32_t GetDepth() { return depth; };

        VectorVector2& GetVectorVector2(uint32_t idx) {
            return ((VectorVector2*)data)[idx];
        }

    private:
        uint32_t width{}, height{}, depth{};
        uint32_t vecWidth{};
        Math::Vector2* data{ nullptr };
    };

    class VectorizedVector3Field : public BaseField {
    public:
        struct VectorVector3 {
            Math::Scalar x[xsimd::simd_type<Math::Scalar>::size];
            Math::Scalar y[xsimd::simd_type<Math::Scalar>::size];
            Math::Scalar z[xsimd::simd_type<Math::Scalar>::size];
        };
    public:
        VectorizedVector3Field() = delete;
        VectorizedVector3Field(uint32_t width, uint32_t height, uint32_t depth) :
                width{ width }, height{ height }, depth{ depth } {
            vecWidth = width + width % xsimd::simd_type<Math::Scalar>::size;
            data = (Math::Vector3*)xsimd::aligned_malloc(vecWidth * height * depth * sizeof(Math::Vector3),
                                                         xsimd::default_arch::alignment());
            Profiler::RecordAllocation(vecWidth * height * depth * sizeof(Math::Vector3), data);
        }
        ~VectorizedVector3Field() final {
            if (data) {
                Profiler::RecordDeallocation(data);
                xsimd::aligned_free(data);
            }
        }

        Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv3Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector3& vv3 = GetVectorVector3(vv3Idx);
            return vv3.x[cIdx];
        }

        Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv3Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector3& vv3 = GetVectorVector3(vv3Idx);
            return Math::Vector2{ vv3.x[cIdx], vv3.y[cIdx] };
        }

        Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv3Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector3& vv3 = GetVectorVector3(vv3Idx);
            return Math::Vector3{ vv3.x[cIdx], vv3.y[cIdx], vv3.z[cIdx] };
        }

        Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv3Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector3& vv3 = GetVectorVector3(vv3Idx);
            return Math::Vector4{ vv3.x[cIdx], vv3.y[cIdx], vv3.z[cIdx], vv3.z[cIdx] };
        }

        xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv3Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector3& vv3 = GetVectorVector3(vv3Idx);
            xsimd::batch<Math::Scalar> r = xsimd::load_aligned(vv3.x);
            return r;
        };

        VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv3Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector3& vv3 = GetVectorVector3(vv3Idx);
            VectorBatch<Math::Scalar, 2> r{};
            r.c[0] = xsimd::load_aligned(vv3.x);
            r.c[1] = xsimd::load_aligned(vv3.y);
            return r;
        };

        VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv3Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector3& vv3 = GetVectorVector3(vv3Idx);
            VectorBatch<Math::Scalar, 3> r{};
            r.c[0] = xsimd::load_aligned(vv3.x);
            r.c[1] = xsimd::load_aligned(vv3.y);
            r.c[2] = xsimd::load_aligned(vv3.z);
            return r;
        };

        VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) final {

            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv3Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector3& vv3 = GetVectorVector3(vv3Idx);
            VectorBatch<Math::Scalar, 4> r{};
            r.c[0] = xsimd::load_aligned(vv3.x);
            r.c[1] = xsimd::load_aligned(vv3.y);
            r.c[2] = xsimd::load_aligned(vv3.z);
            r.c[3] = xsimd::load_aligned(vv3.z);
            return r;
        };

        uint32_t GetComponentCount() final {
            return 3;
        }

        uint32_t GetWidth() { return width; };
        uint32_t GetHeight() { return height; };
        uint32_t GetDepth() { return depth; };

        VectorVector3& GetVectorVector3(uint32_t idx) {
            return ((VectorVector3*)data)[idx];
        }

    private:
        uint32_t width{}, height{}, depth{};
        uint32_t vecWidth{};
        Math::Vector3* data{ nullptr };
    };


    class VectorizedVector4Field : public BaseField {
    public:
        struct VectorVector4 {
            Math::Scalar x[xsimd::simd_type<Math::Scalar>::size];
            Math::Scalar y[xsimd::simd_type<Math::Scalar>::size];
            Math::Scalar z[xsimd::simd_type<Math::Scalar>::size];
            Math::Scalar w[xsimd::simd_type<Math::Scalar>::size];
        };
    public:
        VectorizedVector4Field() = delete;
        VectorizedVector4Field(uint32_t width, uint32_t height, uint32_t depth) :
                width{ width }, height{ height }, depth{ depth } {
            vecWidth = width + width % xsimd::simd_type<Math::Scalar>::size;
            data = (Math::Vector4*)xsimd::aligned_malloc(vecWidth * height * depth * sizeof(Math::Vector4),
                                                         xsimd::default_arch::alignment());
            Profiler::RecordAllocation(vecWidth * height * depth * sizeof(Math::Vector4), data);
        }
        ~VectorizedVector4Field() final {
            if (data) {
                Profiler::RecordDeallocation(data);
                xsimd::aligned_free(data);
            }
        }

        Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv4Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector4& vv4 = GetVectorVector4(vv4Idx);
            return vv4.x[cIdx];
        }

        Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv4Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector4& vv4 = GetVectorVector4(vv4Idx);
            return Math::Vector2{ vv4.x[cIdx], vv4.y[cIdx] };
        }

        Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv4Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector4& vv4 = GetVectorVector4(vv4Idx);
            return Math::Vector3{ vv4.x[cIdx], vv4.y[cIdx], vv4.z[cIdx] };
        }

        Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv4Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            uint32_t cIdx = idx % xsimd::simd_type<Math::Scalar>::size;
            VectorVector4& vv4 = GetVectorVector4(vv4Idx);
            return Math::Vector4{ vv4.x[cIdx], vv4.y[cIdx], vv4.z[cIdx], vv4.w[cIdx] };
        }

        xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv4Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector4& vv4 = GetVectorVector4(vv4Idx);
            xsimd::batch<Math::Scalar> r = xsimd::load_aligned(vv4.x);
            return r;
        };

        VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv4Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector4& vv4 = GetVectorVector4(vv4Idx);
            VectorBatch<Math::Scalar, 2> r{};
            r.c[0] = xsimd::load_aligned(vv4.x);
            r.c[1] = xsimd::load_aligned(vv4.y);
            return r;
        };

        VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) final {
            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv4Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector4& vv4 = GetVectorVector4(vv4Idx);
            VectorBatch<Math::Scalar, 3> r{};
            r.c[0] = xsimd::load_aligned(vv4.x);
            r.c[1] = xsimd::load_aligned(vv4.y);
            r.c[2] = xsimd::load_aligned(vv4.z);
            return r;
        };

        VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) final {

            uint32_t idx = x + y * vecWidth + z * vecWidth * height;
            uint32_t vv4Idx = idx / xsimd::simd_type<Math::Scalar>::size;
            VectorVector4& vv4 = GetVectorVector4(vv4Idx);
            VectorBatch<Math::Scalar, 4> r{};
            r.c[0] = xsimd::load_aligned(vv4.x);
            r.c[1] = xsimd::load_aligned(vv4.y);
            r.c[2] = xsimd::load_aligned(vv4.z);
            r.c[3] = xsimd::load_aligned(vv4.w);
            return r;
        };

        uint32_t GetComponentCount() final {
            return 3;
        }

        uint32_t GetWidth() { return width; };
        uint32_t GetHeight() { return height; };
        uint32_t GetDepth() { return depth; };

        VectorVector4& GetVectorVector4(uint32_t idx) {
            return ((VectorVector4*)data)[idx];
        }

    private:
        uint32_t width{}, height{}, depth{};
        uint32_t vecWidth{};
        Math::Vector4* data{ nullptr };
    };

}