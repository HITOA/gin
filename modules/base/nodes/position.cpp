#include "position.hpp"

#include <gin/math/math.hpp>

Gin::Base::Position::Position()
{
	AddOutputPort("Position", position);
}

void Gin::Base::Position::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		position[idx] = Eigen::Vector3<double>{ (double)x,(double)y,(double)z } * ctx.scale - ctx.bounds.extent;
	});
}

std::string Gin::Base::Position::GetName()
{
	return "Position";
}
