#include <gin/graph/serialization/graphserdes.hpp>
#include <fstream>
#include <gin/module/registry.hpp>

void Gin::Graph::Serialization::SerializeGraph(Graph& graph, SerializedGraph& serializedGraph)
{
	serializedGraph.graphData = nlohmann::json::object();

	serializedGraph.nodes.resize(graph.GetNodeCount());
	serializedGraph.nodesData.resize(graph.GetNodeCount());

	for (size_t i = 0; i < graph.GetNodeCount(); ++i) {
		GraphNodeOperator<Node>& node = graph.GetNode<Node>(i);

		serializedGraph.nodes[i] = node->GetPath();
		serializedGraph.nodesData[i] = nlohmann::json::object();
	}

	auto& adj = graph.GetAdj();

	for (size_t nodeIdx = 0; nodeIdx < graph.GetNodeCount(); ++nodeIdx) {
		Gin::Graph::GraphNodeOperator<Gin::Graph::Node>& node = graph.GetNode<Gin::Graph::Node>(nodeIdx);

		for (size_t portIdx = 0; portIdx < node->GetInputPortCount(); ++portIdx) {
			for (std::pair<size_t, size_t>& link : adj[nodeIdx][portIdx]) {
				serializedGraph.links.push_back(nodeIdx);
				serializedGraph.links.push_back(portIdx);
				serializedGraph.links.push_back(link.first);
				serializedGraph.links.push_back(link.second);
			}
		}
	}

	for (size_t portIdx = 0; portIdx < graph.GetOutputsCount(); ++portIdx) {
		auto& port = graph.GetOutputPort(portIdx);

		for (std::pair<size_t, size_t>& link : port.GetLinks()) {
			serializedGraph.links.push_back(std::numeric_limits<int>::max() - 1);
			serializedGraph.links.push_back(portIdx);
			serializedGraph.links.push_back(link.first);
			serializedGraph.links.push_back(link.second);
		}
	}
}

void Gin::Graph::Serialization::DeserializeGraph(Graph& graph, SerializedGraph& serializedGraph)
{
	for (size_t i = 0; i < serializedGraph.nodes.size(); ++i) {
		std::string& nodePath = serializedGraph.nodes[i];

		Module::GetNodeRegistry()[nodePath](graph, nodePath);
	}
}

void Gin::Graph::Serialization::SaveSerializedGraphToFile(SerializedGraph& serializedGraph, std::string path) {
	nlohmann::json jsonGraph{};

	jsonGraph["name"] = serializedGraph.graphName;
	jsonGraph["data"] = serializedGraph.graphData;

	nlohmann::json jsonNodes{ nlohmann::json::array() };
	
	for (size_t i = 0; i < serializedGraph.nodes.size(); ++i) {
		jsonNodes[i]["path"] = serializedGraph.nodes[i];
		jsonNodes[i]["data"] = serializedGraph.nodesData[i];
	}

	jsonGraph["nodes"] = jsonNodes;
	jsonGraph["links"] = serializedGraph.links;

	std::ofstream f{ path };
	f << jsonGraph.dump(4);
	f.close();
}

void Gin::Graph::Serialization::LoadSerializedGraphFromFile(SerializedGraph& serializedGraph, std::string path)
{
	std::ifstream f{ path };
	nlohmann::json jsonGraph{ nlohmann::json::parse(f) };
	f.close();

	serializedGraph.graphName = jsonGraph["name"];
	serializedGraph.graphData = jsonGraph["data"];

	nlohmann::json jsonNodes{ jsonGraph["nodes"] };
	
	serializedGraph.nodes.resize(jsonNodes.size());
	serializedGraph.nodesData.resize(jsonNodes.size());

	for (size_t i = 0; i < jsonNodes.size(); ++i) {
		serializedGraph.nodes[i] = jsonNodes[i]["path"];
		serializedGraph.nodesData[i] = jsonNodes[i]["data"];
	}

	serializedGraph.links.resize(jsonGraph["links"].size());

	for (size_t i = 0; i < jsonGraph["links"].size(); ++i) {
		serializedGraph.links[i] = jsonGraph["links"][i];
	}
}
