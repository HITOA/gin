#include "gin/graph/serialization/graphserdes.hpp"
#include <fstream>
#include "gin/module/registry.hpp"

nlohmann::json SerializeGraphPort(Gin::Graph::GraphPort& port) {
	nlohmann::json serializedPort{ nlohmann::json::object() };

	serializedPort["type"] = port.GetType().info.get().hash_code();
	serializedPort["name"] = port.GetName();

	return serializedPort;
}

void DeserializeGraphOutputPort(nlohmann::json& serializedPort, Gin::Graph::Graph& graph) {
	size_t typeHashCode = serializedPort["type"];

	//Number

	if (typeHashCode == typeid(int).hash_code()) {
		graph.AddOutput<int>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(float).hash_code()) {
		graph.AddOutput<float>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(double).hash_code()) {
		graph.AddOutput<double>(serializedPort["name"]);
		return;
	}

	//Vec2

	if (typeHashCode == typeid(Eigen::Vector2<int>).hash_code()) {
		graph.AddOutput<Eigen::Vector2<int>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Eigen::Vector2<float>).hash_code()) {
		graph.AddOutput<Eigen::Vector2<float>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Eigen::Vector2<double>).hash_code()) {
		graph.AddOutput<Eigen::Vector2<double>>(serializedPort["name"]);
		return;
	}

	//Vec3

	if (typeHashCode == typeid(Eigen::Vector3<int>).hash_code()) {
		graph.AddOutput<Eigen::Vector3<int>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Eigen::Vector3<float>).hash_code()) {
		graph.AddOutput<Eigen::Vector3<float>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Eigen::Vector3<double>).hash_code()) {
		graph.AddOutput<Eigen::Vector3<double>>(serializedPort["name"]);
		return;
	}

	//Color

	if (typeHashCode == typeid(Eigen::Vector4<float>).hash_code()) {
		graph.AddOutput<Eigen::Vector4<float>>(serializedPort["name"]);
		return;
	}

	//Spatial Number

	if (typeHashCode == typeid(Gin::Spatial::Spatial<int>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<int>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Gin::Spatial::Spatial<float>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<float>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Gin::Spatial::Spatial<double>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<double>>(serializedPort["name"]);
		return;
	}

	//Spatial Vec2

	if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector2<int>>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector2<int>>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector2<float>>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector2<float>>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector2<double>>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector2<double>>>(serializedPort["name"]);
		return;
	}

	//Spatial Vec3

	if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector3<int>>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector3<int>>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector3<float>>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector3<float>>>(serializedPort["name"]);
		return;
	}
	if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector3<double>>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector3<double>>>(serializedPort["name"]);
		return;
	}

	//Spatial Color

	if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector4<float>>).hash_code()) {
		graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector4<float>>>(serializedPort["name"]);
		return;
	}
}

void Gin::Graph::Serialization::SerializeGraph(Graph& graph, SerializedGraph& serializedGraph)
{
	serializedGraph.graphData = nlohmann::json::object();

	serializedGraph.nodes.resize(graph.GetNodeCount());
	serializedGraph.nodesData.resize(graph.GetNodeCount());

	for (size_t i = 0; i < graph.GetNodeCount(); ++i) {
		GraphNodeOperator<Node> node = graph.GetNode<Node>(i);

		serializedGraph.nodes[i] = node->GetPath();
		serializedGraph.nodesData[i] = nlohmann::json::object();
		serializedGraph.nodesData[i]["node"] = node->Serialize();
	}

	for (size_t i = 0; i < graph.GetInputsCount(); ++i) {
		nlohmann::json serializedPort = SerializeGraphPort(graph.GetInputPort(i));
		serializedGraph.graphData["inputs"][i] = serializedPort;
	}

	for (size_t i = 0; i < graph.GetOutputsCount(); ++i) {
		nlohmann::json serializedPort = SerializeGraphPort(graph.GetOutputPort(i));
		serializedGraph.graphData["outputs"][i] = serializedPort;
	}

	auto& adj = graph.GetAdj();

	for (size_t nodeIdx = 0; nodeIdx < graph.GetNodeCount(); ++nodeIdx) {
		Gin::Graph::GraphNodeOperator<Gin::Graph::Node> node = graph.GetNode<Gin::Graph::Node>(nodeIdx);

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
		
		if (Module::GetNodeRegistry().count(nodePath)) {
			size_t nodeIdx = Module::GetNodeRegistry()[nodePath](graph, nodePath);
			graph.GetNode<Node>(nodeIdx)->Deserialize(serializedGraph.nodesData[i]["node"]);
		}
	}

	for (size_t i = 0; i < serializedGraph.graphData["outputs"].size(); ++i)
		DeserializeGraphOutputPort(serializedGraph.graphData["outputs"][i], graph);

	for (size_t i = 0; i < serializedGraph.links.size(); i += 4) {
		size_t nodeAIdx = serializedGraph.links[i];
		size_t portAIdx = serializedGraph.links[i + 1];
		size_t nodeBIdx = serializedGraph.links[i + 2];
		size_t portBIdx = serializedGraph.links[i + 3];

		if (nodeBIdx == std::numeric_limits<int>::max()) {
			graph.GetNode<Node>(nodeAIdx).GetPort(portAIdx).LinkGraphInput(portBIdx);
		}
		else if (nodeAIdx == std::numeric_limits<int>::max() - 1) {
			graph.GetNode<Node>(nodeBIdx).GetPort(portBIdx).LinkGraphOutput(portAIdx);
		}
		else {
			graph.GetNode<Node>(nodeAIdx).GetPort(portAIdx).Link(graph.GetNode<Node>(nodeBIdx).GetPort(portBIdx));
		}
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

void Gin::Graph::Serialization::LoadSerializedGraphFromJson(SerializedGraph& serializedGraph, nlohmann::json json)
{
	serializedGraph.graphName = json["name"];
	serializedGraph.graphData = json["data"];

	nlohmann::json jsonNodes{ json["nodes"] };

	serializedGraph.nodes.resize(jsonNodes.size());
	serializedGraph.nodesData.resize(jsonNodes.size());

	for (size_t i = 0; i < jsonNodes.size(); ++i) {
		serializedGraph.nodes[i] = jsonNodes[i]["path"];
		serializedGraph.nodesData[i] = jsonNodes[i]["data"];
	}

	serializedGraph.links.resize(json["links"].size());

	for (size_t i = 0; i < json["links"].size(); ++i) {
		serializedGraph.links[i] = json["links"][i];
	}
}
