#pragma once

#include <gin/math/bounds.hpp>

namespace Gin::Graph {

	struct GraphContext {
		float scale{ 1.0 };
		Math::Bounds<double, 3> bounds{};
	};

}