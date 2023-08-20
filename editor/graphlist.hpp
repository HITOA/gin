#pragma once

#include <gin/graph/graph.hpp>
#include <gin/graph/serialization/graphserdes.hpp>
#include <imgui.h>

struct GraphListEntry
{
	std::string path{ "" };
	std::string name{ "<new graph>" };
	std::shared_ptr<Gin::Graph::Graph> graph{};
	std::vector<ImVec2> positions{};
};

std::vector<std::shared_ptr<GraphListEntry>>& GetGraphList();