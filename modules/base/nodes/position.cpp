#include "position.hpp"

#include <gin/math/math.hpp>

Gin::Module::Base::Position::Position()
{
	AddOutputPort("Position", position);
}

void Gin::Module::Base::Position::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		position[idx] = Eigen::Vector3<double>{ (double)x,(double)y,(double)z } * ctx.scale - ctx.bounds.extent + ctx.bounds.origin;
	});
}

std::string Gin::Module::Base::Position::GetName()
{
	return "Position";
}
