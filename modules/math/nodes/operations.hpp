#pragma once

#include <gin/graph/node.hpp>
#include <gin/math/type.hpp>
#include <gin/field/sampler.hpp>

namespace Gin::Module::Math {

	class Multiply : public Graph::Node {
	public:
		Multiply() {
			AddInputPort("A", a);
			AddInputPort("B", b);

			AddOutputPort("Result", r);
		}

        virtual void Initialize(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            switch (cc) {
                case 1:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
                case 2:
                    r.SetField(std::make_shared<Field::VectorizedVector2Field>(size.x, size.y, size.z));
                    break;
                case 3:
                    r.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
                    break;
                case 4:
                    r.SetField(std::make_shared<Field::VectorizedVector4Field>(size.x, size.y, size.z));
                    break;
                default:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
            }
        }

		virtual void Execute(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
            size_t idx = 0;

            switch (cc) {
                case 1: {
                    std::shared_ptr<Field::ScalarField<float>> f = r.GetField<Field::ScalarField<float>>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                xsimd::store_aligned(&(*f)[idx], a.GetScalarBatch(x, y, z) * b.GetScalarBatch(x, y, z));
                                idx += simdSizeW;
                            }
                        }
                    }
                    break;
                }
                case 2: {
                    std::shared_ptr<Field::VectorizedVector2Field> f = r.GetField<Field::VectorizedVector2Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 2> av = a.GetVector2Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 2> bv = b.GetVector2Batch(x, y, z);
                                Field::VectorizedVector2Field::VectorVector2& vv2 = f->GetVectorVector2(idx);
                                xsimd::store_aligned(vv2.x, av.c[0] * bv.c[0]);
                                xsimd::store_aligned(vv2.y, av.c[1] * bv.c[1]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    std::shared_ptr<Field::VectorizedVector3Field> f = r.GetField<Field::VectorizedVector3Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 3> av = a.GetVector3Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 3> bv = b.GetVector3Batch(x, y, z);
                                Field::VectorizedVector3Field::VectorVector3 &vv3 = f->GetVectorVector3(idx);
                                xsimd::store_aligned(vv3.x, av.c[0] * bv.c[0]);
                                xsimd::store_aligned(vv3.y, av.c[1] * bv.c[1]);
                                xsimd::store_aligned(vv3.z, av.c[2] * bv.c[2]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 4: {
                    std::shared_ptr<Field::VectorizedVector4Field> f = r.GetField<Field::VectorizedVector4Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 4> av = a.GetVector4Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 4> bv = b.GetVector4Batch(x, y, z);
                                Field::VectorizedVector4Field::VectorVector4 &vv4 = f->GetVectorVector4(idx);
                                xsimd::store_aligned(vv4.x, av.c[0] * bv.c[0]);
                                xsimd::store_aligned(vv4.y, av.c[1] * bv.c[1]);
                                xsimd::store_aligned(vv4.z, av.c[2] * bv.c[2]);
                                xsimd::store_aligned(vv4.w, av.c[3] * bv.c[3]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
		}

		virtual std::string GetName() final {
			return "Multiply";
		}

	private:
        Field::DynamicSampler a{};
        Field::DynamicSampler b{};

        Field::DynamicSampler r{};
	};

    class Add : public Graph::Node {
    public:
        Add() {
            AddInputPort("A", a);
            AddInputPort("B", b);

            AddOutputPort("Result", r);
        }

        virtual void Initialize(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            switch (cc) {
                case 1:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
                case 2:
                    r.SetField(std::make_shared<Field::VectorizedVector2Field>(size.x, size.y, size.z));
                    break;
                case 3:
                    r.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
                    break;
                case 4:
                    r.SetField(std::make_shared<Field::VectorizedVector4Field>(size.x, size.y, size.z));
                    break;
                default:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
            }
        }

        virtual void Execute(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
            size_t idx = 0;

            switch (cc) {
                case 1: {
                    std::shared_ptr<Field::ScalarField<float>> f = r.GetField<Field::ScalarField<float>>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                xsimd::store_aligned(&(*f)[idx], a.GetScalarBatch(x, y, z) + b.GetScalarBatch(x, y, z));
                                idx += simdSizeW;
                            }
                        }
                    }
                    break;
                }
                case 2: {
                    std::shared_ptr<Field::VectorizedVector2Field> f = r.GetField<Field::VectorizedVector2Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 2> av = a.GetVector2Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 2> bv = b.GetVector2Batch(x, y, z);
                                Field::VectorizedVector2Field::VectorVector2& vv2 = f->GetVectorVector2(idx);
                                xsimd::store_aligned(vv2.x, av.c[0] + bv.c[0]);
                                xsimd::store_aligned(vv2.y, av.c[1] + bv.c[1]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    std::shared_ptr<Field::VectorizedVector3Field> f = r.GetField<Field::VectorizedVector3Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 3> av = a.GetVector3Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 3> bv = b.GetVector3Batch(x, y, z);
                                Field::VectorizedVector3Field::VectorVector3 &vv3 = f->GetVectorVector3(idx);
                                xsimd::store_aligned(vv3.x, av.c[0] + bv.c[0]);
                                xsimd::store_aligned(vv3.y, av.c[1] + bv.c[1]);
                                xsimd::store_aligned(vv3.z, av.c[2] + bv.c[2]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 4: {
                    std::shared_ptr<Field::VectorizedVector4Field> f = r.GetField<Field::VectorizedVector4Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 4> av = a.GetVector4Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 4> bv = b.GetVector4Batch(x, y, z);
                                Field::VectorizedVector4Field::VectorVector4 &vv4 = f->GetVectorVector4(idx);
                                xsimd::store_aligned(vv4.x, av.c[0] + bv.c[0]);
                                xsimd::store_aligned(vv4.y, av.c[1] + bv.c[1]);
                                xsimd::store_aligned(vv4.z, av.c[2] + bv.c[2]);
                                xsimd::store_aligned(vv4.w, av.c[3] + bv.c[3]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        virtual std::string GetName() final {
            return "Add";
        }

    private:
        Field::DynamicSampler a{};
        Field::DynamicSampler b{};

        Field::DynamicSampler r{};
    };

    class Substract : public Graph::Node {
    public:
        Substract() {
            AddInputPort("A", a);
            AddInputPort("B", b);

            AddOutputPort("Result", r);
        }

        virtual void Initialize(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            switch (cc) {
                case 1:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
                case 2:
                    r.SetField(std::make_shared<Field::VectorizedVector2Field>(size.x, size.y, size.z));
                    break;
                case 3:
                    r.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
                    break;
                case 4:
                    r.SetField(std::make_shared<Field::VectorizedVector4Field>(size.x, size.y, size.z));
                    break;
                default:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
            }
        }

        virtual void Execute(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
            size_t idx = 0;

            switch (cc) {
                case 1: {
                    std::shared_ptr<Field::ScalarField<float>> f = r.GetField<Field::ScalarField<float>>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                xsimd::store_aligned(&(*f)[idx], a.GetScalarBatch(x, y, z) - b.GetScalarBatch(x, y, z));
                                idx += simdSizeW;
                            }
                        }
                    }
                    break;
                }
                case 2: {
                    std::shared_ptr<Field::VectorizedVector2Field> f = r.GetField<Field::VectorizedVector2Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 2> av = a.GetVector2Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 2> bv = b.GetVector2Batch(x, y, z);
                                Field::VectorizedVector2Field::VectorVector2& vv2 = f->GetVectorVector2(idx);
                                xsimd::store_aligned(vv2.x, av.c[0] - bv.c[0]);
                                xsimd::store_aligned(vv2.y, av.c[1] - bv.c[1]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    std::shared_ptr<Field::VectorizedVector3Field> f = r.GetField<Field::VectorizedVector3Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 3> av = a.GetVector3Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 3> bv = b.GetVector3Batch(x, y, z);
                                Field::VectorizedVector3Field::VectorVector3 &vv3 = f->GetVectorVector3(idx);
                                xsimd::store_aligned(vv3.x, av.c[0] - bv.c[0]);
                                xsimd::store_aligned(vv3.y, av.c[1] - bv.c[1]);
                                xsimd::store_aligned(vv3.z, av.c[2] - bv.c[2]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 4: {
                    std::shared_ptr<Field::VectorizedVector4Field> f = r.GetField<Field::VectorizedVector4Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 4> av = a.GetVector4Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 4> bv = b.GetVector4Batch(x, y, z);
                                Field::VectorizedVector4Field::VectorVector4 &vv4 = f->GetVectorVector4(idx);
                                xsimd::store_aligned(vv4.x, av.c[0] - bv.c[0]);
                                xsimd::store_aligned(vv4.y, av.c[1] - bv.c[1]);
                                xsimd::store_aligned(vv4.z, av.c[2] - bv.c[2]);
                                xsimd::store_aligned(vv4.w, av.c[3] - bv.c[3]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        virtual std::string GetName() final {
            return "Multiply";
        }

    private:
        Field::DynamicSampler a{};
        Field::DynamicSampler b{};

        Field::DynamicSampler r{};
    };

    class Divide : public Graph::Node {
    public:
        Divide() {
            AddInputPort("A", a);
            AddInputPort("B", b);

            AddOutputPort("Result", r);
        }

        virtual void Initialize(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            switch (cc) {
                case 1:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
                case 2:
                    r.SetField(std::make_shared<Field::VectorizedVector2Field>(size.x, size.y, size.z));
                    break;
                case 3:
                    r.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
                    break;
                case 4:
                    r.SetField(std::make_shared<Field::VectorizedVector4Field>(size.x, size.y, size.z));
                    break;
                default:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
            }
        }

        virtual void Execute(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
            size_t idx = 0;

            switch (cc) {
                case 1: {
                    std::shared_ptr<Field::ScalarField<float>> f = r.GetField<Field::ScalarField<float>>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                xsimd::store_aligned(&(*f)[idx], a.GetScalarBatch(x, y, z) / b.GetScalarBatch(x, y, z));
                                idx += simdSizeW;
                            }
                        }
                    }
                    break;
                }
                case 2: {
                    std::shared_ptr<Field::VectorizedVector2Field> f = r.GetField<Field::VectorizedVector2Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 2> av = a.GetVector2Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 2> bv = b.GetVector2Batch(x, y, z);
                                Field::VectorizedVector2Field::VectorVector2& vv2 = f->GetVectorVector2(idx);
                                xsimd::store_aligned(vv2.x, av.c[0] / bv.c[0]);
                                xsimd::store_aligned(vv2.y, av.c[1] / bv.c[1]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    std::shared_ptr<Field::VectorizedVector3Field> f = r.GetField<Field::VectorizedVector3Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 3> av = a.GetVector3Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 3> bv = b.GetVector3Batch(x, y, z);
                                Field::VectorizedVector3Field::VectorVector3 &vv3 = f->GetVectorVector3(idx);
                                xsimd::store_aligned(vv3.x, av.c[0] / bv.c[0]);
                                xsimd::store_aligned(vv3.y, av.c[1] / bv.c[1]);
                                xsimd::store_aligned(vv3.z, av.c[2] / bv.c[2]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 4: {
                    std::shared_ptr<Field::VectorizedVector4Field> f = r.GetField<Field::VectorizedVector4Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 4> av = a.GetVector4Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 4> bv = b.GetVector4Batch(x, y, z);
                                Field::VectorizedVector4Field::VectorVector4 &vv4 = f->GetVectorVector4(idx);
                                xsimd::store_aligned(vv4.x, av.c[0] / bv.c[0]);
                                xsimd::store_aligned(vv4.y, av.c[1] / bv.c[1]);
                                xsimd::store_aligned(vv4.z, av.c[2] / bv.c[2]);
                                xsimd::store_aligned(vv4.w, av.c[3] / bv.c[3]);
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        virtual std::string GetName() final {
            return "Multiply";
        }

    private:
        Field::DynamicSampler a{};
        Field::DynamicSampler b{};

        Field::DynamicSampler r{};
    };

    class Pow : public Graph::Node {
    public:
        Pow() {
            AddInputPort("A", a);
            AddInputPort("B", b);

            AddOutputPort("Result", r);
        }

        virtual void Initialize(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            switch (cc) {
                case 1:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
                case 2:
                    r.SetField(std::make_shared<Field::VectorizedVector2Field>(size.x, size.y, size.z));
                    break;
                case 3:
                    r.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
                    break;
                case 4:
                    r.SetField(std::make_shared<Field::VectorizedVector4Field>(size.x, size.y, size.z));
                    break;
                default:
                    r.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
                    break;
            }
        }

        virtual void Execute(Graph::GraphContext ctx) final {
            Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

            uint32_t cc = std::max(a.GetComponentCount(), b.GetComponentCount());

            constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
            size_t idx = 0;

            switch (cc) {
                case 1: {
                    std::shared_ptr<Field::ScalarField<float>> f = r.GetField<Field::ScalarField<float>>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                xsimd::store_aligned(&(*f)[idx], xsimd::pow(a.GetScalarBatch(x, y, z), b.GetScalarBatch(x, y, z)));
                                idx += simdSizeW;
                            }
                        }
                    }
                    break;
                }
                case 2: {
                    std::shared_ptr<Field::VectorizedVector2Field> f = r.GetField<Field::VectorizedVector2Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 2> av = a.GetVector2Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 2> bv = b.GetVector2Batch(x, y, z);
                                Field::VectorizedVector2Field::VectorVector2& vv2 = f->GetVectorVector2(idx);
                                xsimd::store_aligned(vv2.x, xsimd::pow(av.c[0], bv.c[0]));
                                xsimd::store_aligned(vv2.y, xsimd::pow(av.c[1], bv.c[1]));
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    std::shared_ptr<Field::VectorizedVector3Field> f = r.GetField<Field::VectorizedVector3Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 3> av = a.GetVector3Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 3> bv = b.GetVector3Batch(x, y, z);
                                Field::VectorizedVector3Field::VectorVector3 &vv3 = f->GetVectorVector3(idx);
                                xsimd::store_aligned(vv3.x, xsimd::pow(av.c[0], bv.c[0]));
                                xsimd::store_aligned(vv3.y, xsimd::pow(av.c[1], bv.c[1]));
                                xsimd::store_aligned(vv3.z, xsimd::pow(av.c[2], bv.c[2]));
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                case 4: {
                    std::shared_ptr<Field::VectorizedVector4Field> f = r.GetField<Field::VectorizedVector4Field>();
                    for (size_t z = 0; z < size.z; ++z) {
                        for (size_t y = 0; y < size.y; ++y) {
                            for (size_t x = 0; x < size.x; x += simdSizeW) {
                                Field::VectorBatch<Gin::Math::Scalar, 4> av = a.GetVector4Batch(x, y, z);
                                Field::VectorBatch<Gin::Math::Scalar, 4> bv = b.GetVector4Batch(x, y, z);
                                Field::VectorizedVector4Field::VectorVector4 &vv4 = f->GetVectorVector4(idx);
                                xsimd::store_aligned(vv4.x, xsimd::pow(av.c[0], bv.c[0]));
                                xsimd::store_aligned(vv4.y, xsimd::pow(av.c[1], bv.c[1]));
                                xsimd::store_aligned(vv4.z, xsimd::pow(av.c[2], bv.c[2]));
                                xsimd::store_aligned(vv4.w, xsimd::pow(av.c[3], bv.c[3]));
                                ++idx;
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        virtual std::string GetName() final {
            return "Multiply";
        }

    private:
        Field::DynamicSampler a{};
        Field::DynamicSampler b{};

        Field::DynamicSampler r{};
    };

}