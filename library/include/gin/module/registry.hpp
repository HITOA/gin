#pragma once

#include <gin/graph/graph.hpp>
#include <vector>
#include <module.hpp>

namespace Gin::Module {
	std::map<std::string, std::function<size_t(Graph::Graph&, std::string)>>& GetNodeRegistry();
	
	template<typename T>
	inline void AddNodesToRegistry(const std::string& path) {
		GetNodeRegistry()[path] = [](Graph::Graph& graph, std::string path) {
			std::shared_ptr<T> node{ std::make_shared<T>() };
			node->SetPath(path);
			return graph.AddNode<T>(node).GetNodeId();
		};
	}
}