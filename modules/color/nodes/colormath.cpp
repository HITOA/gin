#include "colormath.hpp"
#include <gin/math/interpolation.hpp>

/*
Gin::Module::Color::Blend::Blend()
{
	AddInputPort("A", a);
	AddInputPort("B", b);
	AddInputPort("T", t);

	AddOutputPort("Color", color);
}

void Gin::Module::Color::Blend::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		color[idx] = Math::Linear<Eigen::Vector4<float>>(a[idx], b[idx], t[idx]);
	});
}

void Gin::Module::Color::Blend::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		color[idx] = Math::Linear<Eigen::Vector4<float>>(a[idx], b[idx], t[idx]);
	});
}

std::string Gin::Module::Color::Blend::GetName()
{
	return "Blend";
}*/
