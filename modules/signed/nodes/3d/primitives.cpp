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

Gin::Signed::SDGround::SDGround()
{
	AddInputPort("Height", height);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Signed::SDGround::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distance[idx] = position[idx].y() - height[idx];
	});
}

std::string Gin::Signed::SDGround::GetName()
{
	return "SDGround";
}

Gin::Signed::SDPlane::SDPlane()
{
	AddInputPort("Height", height);
	AddInputPort("Normal", normal);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Signed::SDPlane::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distance[idx] = position[idx].dot(normal[idx]) + height[idx];
	});
}

std::string Gin::Signed::SDPlane::GetName()
{
	return "SDPlane";
}
