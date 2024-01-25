#include "functions.hpp"
#include <gin/math/interpolation.hpp>
#include <gin/math/math.hpp>
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