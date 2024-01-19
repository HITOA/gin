#pragma once

#include <gin/math/bounds.hpp>

namespace Gin::Graph {

	struct GraphContext {
		float scale{ 1.0 };
		Math::Bounds<double, 3> bounds{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } };
	};

}