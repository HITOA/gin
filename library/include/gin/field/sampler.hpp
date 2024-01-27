#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <gin/field/field.hpp>
#include <gin/math/type.hpp>
#include <gin/field/constantfield.hpp>
#include <typeindex>

namespace Gin::Field {

    struct Dynamic {};

    template<typename T>
    class Sampler {
    public:
        Sampler() {
            SetField(std::make_shared<ConstantField<T>>());
        };
        Sampler(T v) {
            SetField(std::make_shared<ConstantField<T>>(v));
        };
        template<typename U>
        Sampler(const Sampler<U>& s) : field{ s.field }, typeIndex{ s.typeIndex } {};

        template<typename U>
        inline void SetField(std::shared_ptr<U> f) {
            field = f;
            typeIndex = typeid(U);
        }

        template<typename U>
        inline std::shared_ptr<U> GetField() {
            if (typeIndex == typeid(U))
                return std::dynamic_pointer_cast<U>(field);
            return nullptr;
        }

        template<typename U>
        inline bool IsFieldOfType() {
            return typeIndex == typeid(U);
        }

        inline uint32_t GetComponentCount() {
            return field->GetComponentCount();
        }

        inline Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetScalar(x, y, z);
        }

        inline Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector2(x, y, z);
        }

        inline Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector3(x, y, z);
        }

        inline Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector4(x, y, z);
        }

        inline xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetScalarBatch(x, y, z);
        }

        inline VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector2Batch(x, y, z);
        }

        inline VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector3Batch(x, y, z);
        }

        inline VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector4Batch(x, y, z);
        }
    private:
        std::shared_ptr<BaseField> field{};
        std::type_index typeIndex{ typeid(void) };
    };

    template<>
    class Sampler<Dynamic> {
    public:
        Sampler() {
            SetField(std::make_shared<ConstantField<float>>());
        };
        template<typename U>
        Sampler(const Sampler<U>& s) : field{ s.field }, typeIndex{ s.typeIndex } {};

        template<typename U>
        inline void SetField(std::shared_ptr<U> f) {
            field = f;
            typeIndex = typeid(U);
        }

        template<typename U>
        inline std::shared_ptr<U> GetField() {
            if (typeIndex == typeid(U))
                return std::dynamic_pointer_cast<U>(field);
            return nullptr;
        }

        template<typename U>
        inline bool IsFieldOfType() {
            return typeIndex == typeid(U);
        }

        inline uint32_t GetComponentCount() {
            return field->GetComponentCount();
        }

        inline Math::Scalar GetScalar(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetScalar(x, y, z);
        }

        inline Math::Vector2 GetVector2(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector2(x, y, z);
        }

        inline Math::Vector3 GetVector3(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector3(x, y, z);
        }

        inline Math::Vector4 GetVector4(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector4(x, y, z);
        }

        inline xsimd::batch<Math::Scalar> GetScalarBatch(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetScalarBatch(x, y, z);
        }

        inline VectorBatch<Math::Scalar, 2> GetVector2Batch(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector2Batch(x, y, z);
        }

        inline VectorBatch<Math::Scalar, 3> GetVector3Batch(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector3Batch(x, y, z);
        }

        inline VectorBatch<Math::Scalar, 4> GetVector4Batch(uint32_t x, uint32_t y, uint32_t z) {
            return field->GetVector4Batch(x, y, z);
        }
    private:
        std::shared_ptr<BaseField> field{};
        std::type_index typeIndex{ typeid(void) };
    };

    typedef Sampler<Dynamic> DynamicSampler;

}