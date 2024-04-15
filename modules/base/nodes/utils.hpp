#pragma once

#include <gin/gin.hpp>
#include <gin/field/sampler.hpp>
#include <gin/field/field.hpp>

namespace Gin::Module::Base {

	class Combine : public Graph::Node {
	public:
		Combine() {
			AddInputPort("X", x);
			AddInputPort("Y", y);
			AddInputPort("Z", z);
			AddInputPort("W", w);

			AddOutputPort("Vector4", vec4);
		}

        virtual void Initialize(Graph::GraphContext ctx) final {
            if (x.IsFieldOfType<Field::ConstantField<float>>() &&
                y.IsFieldOfType<Field::ConstantField<float>>() &&
                z.IsFieldOfType<Field::ConstantField<float>>() &&
                w.IsFieldOfType<Field::ConstantField<float>>()) {
                vec4.SetField(std::make_shared<Field::ConstantField<Math::Vector4>>());
            } else {
                Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
                vec4.SetField(std::make_shared<Field::VectorizedVector4Field>(size.x, size.y, size.z));
            }
        }

		virtual void Execute(Graph::GraphContext ctx) final {
            if (x.IsFieldOfType<Field::ConstantField<float>>() &&
                    y.IsFieldOfType<Field::ConstantField<float>>() &&
                    z.IsFieldOfType<Field::ConstantField<float>>() &&
                    w.IsFieldOfType<Field::ConstantField<float>>()) {

                vec4.GetField<Field::ConstantField<Math::Vector4>>()->Get() = Math::Vector4{
                    x.GetScalar(0, 0, 0),
                    y.GetScalar(0, 0, 0),
                    z.GetScalar(0, 0, 0),
                    w.GetScalar(0, 0, 0) };

            } else {
                Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

                constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
                size_t idx = 0;

                std::shared_ptr<Field::VectorizedVector4Field> fv4 = vec4.GetField<Field::VectorizedVector4Field>();

                for (size_t _z = 0; _z < size.z; ++_z) {
                    for (size_t _y = 0; _y < size.y; ++_y) {
                        for (size_t _x = 0; _x < size.x; _x += simdSizeW) {
                            Field::VectorizedVector4Field::VectorVector4& vv4 = fv4->GetVectorVector4(idx);
                            xsimd::store_aligned(vv4.x, x.GetScalarBatch(_x, _y, _z));
                            xsimd::store_aligned(vv4.y, y.GetScalarBatch(_x, _y, _z));
                            xsimd::store_aligned(vv4.z, z.GetScalarBatch(_x, _y, _z));
                            xsimd::store_aligned(vv4.w, w.GetScalarBatch(_x, _y, _z));
                            ++idx;
                        }
                    }
                }
            }
		};

		virtual std::string GetName() final {
			return "Combine";
		};

	private:
        Field::Sampler<float> x{};
        Field::Sampler<float> y{};
        Field::Sampler<float> z{};
        Field::Sampler<float> w{};

        Field::Sampler<Math::Vector4> vec4{};
	};

	class Split : public Graph::Node {
	public:
        Split() {
			AddInputPort("A", a);

			AddOutputPort("X", x);
			AddOutputPort("Y", y);
			AddOutputPort("Z", z);
            AddOutputPort("W", w);
		}

        virtual void Initialize(Graph::GraphContext ctx) final {
            if (a.IsFieldOfType<Field::ConstantField<float>>() ||
                    a.IsFieldOfType<Field::ConstantField<Math::Vector2>>() ||
                    a.IsFieldOfType<Field::ConstantField<Math::Vector3>>() ||
                    a.IsFieldOfType<Field::ConstantField<Math::Vector4>>()) {
                x.SetField(std::make_shared<Field::ConstantField<float>>());
                y.SetField(std::make_shared<Field::ConstantField<float>>());
                z.SetField(std::make_shared<Field::ConstantField<float>>());
                w.SetField(std::make_shared<Field::ConstantField<float>>());
            } else {
                Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

                x.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                y.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                z.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                w.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
            }
        }

		virtual void Execute(Graph::GraphContext ctx) final {
            if (a.IsFieldOfType<Field::ConstantField<float>>() ||
                a.IsFieldOfType<Field::ConstantField<Math::Vector2>>() ||
                a.IsFieldOfType<Field::ConstantField<Math::Vector3>>() ||
                a.IsFieldOfType<Field::ConstantField<Math::Vector4>>()) {
                Math::Vector4 v4 = a.GetVector4(0, 0, 0);
                x.GetField<Field::ConstantField<float>>()->Get() = v4.x;
                y.GetField<Field::ConstantField<float>>()->Get() = v4.y;
                z.GetField<Field::ConstantField<float>>()->Get() = v4.z;
                w.GetField<Field::ConstantField<float>>()->Get() = v4.w;

            } else {
                Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

                constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
                size_t idx = 0;

                std::shared_ptr<Field::ScalarField<float>> fx = x.GetField<Field::ScalarField<float>>();
                std::shared_ptr<Field::ScalarField<float>> fy = y.GetField<Field::ScalarField<float>>();
                std::shared_ptr<Field::ScalarField<float>> fz = z.GetField<Field::ScalarField<float>>();
                std::shared_ptr<Field::ScalarField<float>> fw = w.GetField<Field::ScalarField<float>>();

                for (size_t z = 0; z < size.z; ++z) {
                    for (size_t y = 0; y < size.y; ++y) {
                        for (size_t x = 0; x < size.x; x += simdSizeW) {
                            Field::VectorBatch<Math::Scalar, 4> vb4 = a.GetVector4Batch(x, y, z);
                            xsimd::store(&(*fx)[idx], vb4.c[0]);
                            xsimd::store(&(*fy)[idx], vb4.c[1]);
                            xsimd::store(&(*fz)[idx], vb4.c[2]);
                            xsimd::store(&(*fw)[idx], vb4.c[3]);
                            idx += simdSizeW;
                        }
                    }
                }
            }
		};

		virtual std::string GetName() final {
			return "Split";
		};

	private:
        Field::Sampler<Math::Vector4> a{};

        Field::Sampler<float> x{};
        Field::Sampler<float> y{};
        Field::Sampler<float> z{};
        Field::Sampler<float> w{};
	};

}