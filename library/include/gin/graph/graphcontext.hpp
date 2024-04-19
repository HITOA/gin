#pragma once

#include <gin/math/type.hpp>

namespace Gin::Graph {

	struct GraphContext {
		Math::Scalar scale{ 1.0 };
		Math::Bounds bounds{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } };
	};

}