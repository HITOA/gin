#include <signed/nodes/3d/operations.hpp>
#include <gin/math/math.hpp>

Gin::Module::Signed::OPUnion::OPUnion()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);

	AddOutputPort("Signed Distance", distanceR);
}

void Gin::Module::Signed::OPUnion::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distanceR[idx] = Math::Min<float>(distanceA[idx], distanceB[idx]);
	});
}

std::string Gin::Module::Signed::OPUnion::GetName()
{
	return "OPUnion";
}

Gin::Module::Signed::OPSubstraction::OPSubstraction()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);

	AddOutputPort("Signed Distance", distanceR);
}

void Gin::Module::Signed::OPSubstraction::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distanceR[idx] = Math::Max<float>(-distanceA[idx], distanceB[idx]);
	});
}

std::string Gin::Module::Signed::OPSubstraction::GetName()
{
	return "OPSubstraction";
}

Gin::Module::Signed::OPIntersection::OPIntersection()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);

	AddOutputPort("Signed Distance", distanceR);
}

void Gin::Module::Signed::OPIntersection::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distanceR[idx] = Math::Max<float>(distanceA[idx], distanceB[idx]);
	});
}

std::string Gin::Module::Signed::OPIntersection::GetName()
{
	return "OPIntersection";
}

Gin::Module::Signed::OPSmoothUnion::OPSmoothUnion()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);
	AddInputPort("Smooth Factor", smoothFactor);

	AddOutputPort("Signed Distance", distanceR);
}

void Gin::Module::Signed::OPSmoothUnion::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distanceR[idx] = Math::SMin(distanceA[idx], distanceB[idx], smoothFactor[idx]).x();
	});
}

std::string Gin::Module::Signed::OPSmoothUnion::GetName()
{
	return "OPSmoothUnion";
}

Gin::Module::Signed::OPSmoothSubstraction::OPSmoothSubstraction()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);
	AddInputPort("Smooth Factor", smoothFactor);

	AddOutputPort("Signed Distance", distanceR);
}

void Gin::Module::Signed::OPSmoothSubstraction::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distanceR[idx] = Math::SMax(-distanceA[idx], distanceB[idx], smoothFactor[idx]).x();
		});
}

std::string Gin::Module::Signed::OPSmoothSubstraction::GetName()
{
	return "OPSmoothSubstraction";
}

Gin::Module::Signed::OPSmoothIntersection::OPSmoothIntersection()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);
	AddInputPort("Smooth Factor", smoothFactor);

	AddOutputPort("Signed Distance", distanceR);
}

void Gin::Module::Signed::OPSmoothIntersection::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distanceR[idx] = Math::SMax(distanceA[idx], distanceB[idx], smoothFactor[idx]).x();
		});
}

std::string Gin::Module::Signed::OPSmoothIntersection::GetName()
{
	return "OPSmoothIntersection";
}