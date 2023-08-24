#pragma once

#include <vector>
#include <string>
#include <gin/graph/serialization/json.hpp>
#include <gin/graph/graph.hpp>

namespace Gin::Graph::Serialization {

	/**
	 * Serialized Graph Object.
	 */
	struct SerializedGraph {
		std::string graphName{}; //Graph Name
		nlohmann::json graphData{};
		std::vector<std::string> nodes{}; //Nodes path
		std::vector<nlohmann::json> nodesData{}; //Nodes Data
		std::vector<size_t> links{};
	};
	
	void SerializeGraph(Graph& graph, SerializedGraph& serializedGraph);
	void DeserializeGraph(Graph& graph, SerializedGraph& serializedGraph);

	void SaveSerializedGraphToFile(SerializedGraph& serializedGraph, std::string path);
	void LoadSerializedGraphFromFile(SerializedGraph& serializedGraph, std::string path);
	void LoadSerializedGraphFromJson(SerializedGraph& serializedGraph, nlohmann::json json);
}
