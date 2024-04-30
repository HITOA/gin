#include "functions.hpp"
#include <gin/math/interpolation.hpp>
#include <gin/math/math.hpp>

Gin::Module::Math::Smoothstep::Smoothstep() {
    AddInputPort("In", in);
    AddInputPort("Min", min);
    AddInputPort("Max", max);

    AddOutputPort("Out", out);
}

void Gin::Module::Math::Smoothstep::Initialize(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    out.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Math::Smoothstep::Execute(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::ScalarField<float>> o = out.GetField<Field::ScalarField<float>>();

    constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSizeW) {
                auto v = in.GetScalarBatch(x, y, z);
                auto m = min.GetScalarBatch(x, y, z);
                auto M = max.GetScalarBatch(x, y, z);

                auto t = (v - m) / (M - m);
                t = xsimd::min(xsimd::max(t, xsimd::batch<float>{ 0.0 }), xsimd::batch<float>{ 1.0 });
                v = t * t * (3.0 - 2.0 * t);

                xsimd::store_aligned(&(*o)[idx], v);

                idx += simdSizeW;
            }
        }
    }
}

std::string Gin::Module::Math::Smoothstep::GetName() {
    return "Smoothstep";
}

/*
Gin::Module::Math::Lerp::Lerp()
{
	AddInputPort("A", a);
	AddInputPort("B", b);
	AddInputPort("T", t);

	AddOutputPort("Result", r);
}

void Gin::Module::Math::Lerp::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		r[idx] = Gin::Math::Linear<float>(a[idx], b[idx], t[idx]);
	});
}

void Gin::Module::Math::Lerp::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		r[idx] = Gin::Math::Linear<float>(a[idx], b[idx], t[idx]);
	});
}

std::string Gin::Module::Math::Lerp::GetName()
{
	return "Lerp";
}

Gin::Module::Math::InvLerp::InvLerp()
{
	AddInputPort("A", a);
	AddInputPort("B", b);
	AddInputPort("T", t);

	AddOutputPort("Result", r);
}

void Gin::Module::Math::InvLerp::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		r[idx] = Gin::Math::InverseLerp<float>(a[idx], b[idx], t[idx]);
	});
}

void Gin::Module::Math::InvLerp::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		r[idx] = Gin::Math::InverseLerp<float>(a[idx], b[idx], t[idx]);
	});
}

std::string Gin::Module::Math::InvLerp::GetName()
{
	return "InvLerp";
}

Gin::Module::Math::SmoothStep::SmoothStep()
{
	AddInputPort("A", a);
	AddInputPort("Min", min);
	AddInputPort("Max", max);

	AddOutputPort("Result", r);
}

void Gin::Module::Math::SmoothStep::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		float t = Gin::Math::Clamp<float>((a[idx] - min[idx]) / (max[idx] - min[idx]), 0.0f, 1.0f);
		r[idx] = t * t * (3.0 - 2.0 * t);
	});
}

void Gin::Module::Math::SmoothStep::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		float t = Gin::Math::Clamp<float>((a[idx] - min[idx]) / (max[idx] - min[idx]), 0.0f, 1.0f);
		r[idx] = t * t * (3.0 - 2.0 * t);
	});
}

std::string Gin::Module::Math::SmoothStep::GetName()
{
	return "SmoothStep";
}

Gin::Module::Math::Remap::Remap()
{
	AddInputPort("A", a);
	AddInputPort("Min", minA);
	AddInputPort("Max", maxA);
	AddInputPort("New Min", minB);
	AddInputPort("New Max", maxB);

	AddOutputPort("Result", r);
}

void Gin::Module::Math::Remap::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		r[idx] = minB[idx] + (a[idx] - minA[idx]) * (maxB[idx] - minB[idx]) / (maxA[idx] - minA[idx]);
	});
}

void Gin::Module::Math::Remap::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		r[idx] = minB[idx] + (a[idx] - minA[idx]) * (maxB[idx] - minB[idx]) / (maxA[idx] - minA[idx]);
	});
}

std::string Gin::Module::Math::Remap::GetName()
{
	return "Remap";
}
*/