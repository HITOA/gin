#include "features.hpp"

#include <gin/math/interpolation.hpp>
#include <gin/math/math.hpp>
/*
Gin::Module::Terrain::Overhang::Overhang()
{
	AddInputPort("Base Height", base);
	AddInputPort("Amplitude", amplitude);
	AddInputPort("Mask", mask);
	AddInputPort("Mid Level", midLevel);
	AddInputPort("Slop Width", slopWidth);
	AddInputPort("Slop Profile", slopProfile);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Module::Terrain::Overhang::Execute(Gin::Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		float height = pow(Math::Smoothstep<float>(mask[idx], midLevel[idx] - slopWidth[idx], midLevel[idx] + slopWidth[idx]), slopProfile[idx]) * amplitude[idx] * 1.01f;

		float topPlane = position[idx].y() - (base[idx] + amplitude[idx]);
		float bottomPlane = -1 * (position[idx].y() - (base[idx] + amplitude[idx] - height));

		distance[idx] = Math::Max<float>(bottomPlane, topPlane) + 0.1f;
	});
}

void Gin::Module::Terrain::Overhang::Execute(Gin::Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		float height = pow(Math::Smoothstep<float>(mask[idx], midLevel[idx] - slopWidth[idx], midLevel[idx] + slopWidth[idx]), slopProfile[idx]) * amplitude[idx] * 1.01f;

		float topPlane = position[idx].y() - (base[idx] + amplitude[idx]);
		float bottomPlane = -1 * (position[idx].y() - (base[idx] + amplitude[idx] - height));

		distance[idx] = Math::Max<float>(bottomPlane, topPlane) + 0.1f;
	});
}

std::string Gin::Module::Terrain::Overhang::GetName()
{
	return "Overhang";
}
*/