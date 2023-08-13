#include "primitives.hpp"

Gin::Signed::SDSphere::SDSphere()
{
	AddInputPort("Radius", radius);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Signed::SDSphere::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distance[idx] = sqrt(position[idx].dot(position[idx])) - radius[idx];
	});
}

std::string Gin::Signed::SDSphere::GetName()
{
	return "SDSphere";
}
