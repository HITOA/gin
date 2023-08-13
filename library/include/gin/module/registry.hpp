#pragma once

#include <gin/graph/graph.hpp>
#include <vector>
#include <module.hpp>

namespace Gin::Module {
	std::unordered_map<std::string, std::function<size_t(Graph::Graph&)>>& GetRegistry();
}