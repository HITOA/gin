#include "operations.hpp"

#include <gin/math/interpolation.hpp>

Gin::Module::Terrain::TerrainMix::TerrainMix()
{
	AddInputPort("A", a);
	AddInputPort("B", b);
	AddInputPort("Mask", mask);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Module::Terrain::TerrainMix::Execute(Gin::Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		distance[idx] = Math::Linear<float>(a[idx], b[idx], Math::Smoothstep<float>(mask[idx], -1.0f, 1.0f));
	});
}

std::string Gin::Module::Terrain::TerrainMix::GetName()
{
	return "Terrain Mix";
}
