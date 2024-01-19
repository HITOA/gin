#include "gin/graph/serialization/graphserdes.hpp"
#include <fstream>
#include "gin/module/registry.hpp"

size_t GetTypeHashCodeFromId(uint32_t id) {
    switch (id) {
        case 0: return typeid(int).hash_code();
        case 1: return typeid(float).hash_code();
        case 2: return typeid(double).hash_code();
        case 3: return typeid(Eigen::Vector2<int>).hash_code();
        case 4: return typeid(Eigen::Vector2<float>).hash_code();
        case 5: return typeid(Eigen::Vector2<double>).hash_code();
        case 6: return typeid(Eigen::Vector3<int>).hash_code();
        case 7: return typeid(Eigen::Vector3<float>).hash_code();
        case 8: return typeid(Eigen::Vector3<double>).hash_code();
        case 9: return typeid(Eigen::Vector4<float>).hash_code();
        case 10: return typeid(Gin::Spatial::Spatial<int>).hash_code();
        case 11: return typeid(Gin::Spatial::Spatial<float>).hash_code();
        case 12: return typeid(Gin::Spatial::Spatial<double>).hash_code();
        case 13: return typeid(Gin::Spatial::Spatial<Eigen::Vector2<int>>).hash_code();
        case 14: return typeid(Gin::Spatial::Spatial<Eigen::Vector2<float>>).hash_code();
        case 15: return typeid(Gin::Spatial::Spatial<Eigen::Vector2<double>>).hash_code();
        case 16: return typeid(Gin::Spatial::Spatial<Eigen::Vector3<int>>).hash_code();
        case 17: return typeid(Gin::Spatial::Spatial<Eigen::Vector3<float>>).hash_code();
        case 18: return typeid(Gin::Spatial::Spatial<Eigen::Vector3<double>>).hash_code();
        case 19: return typeid(Gin::Spatial::Spatial<Eigen::Vector4<float>>).hash_code();
        default : return typeid(void).hash_code();
    }
}

uint32_t GetTypeIdFromHashCode(size_t typeHashCode) {
    if (typeHashCode == typeid(int).hash_code())
        return 0;
    if (typeHashCode == typeid(float).hash_code())
        return 1;
    if (typeHashCode == typeid(double).hash_code())
        return 2;

    //Vec2

    if (typeHashCode == typeid(Eigen::Vector2<int>).hash_code())
        return 3;
    if (typeHashCode == typeid(Eigen::Vector2<float>).hash_code())
        return 4;
    if (typeHashCode == typeid(Eigen::Vector2<double>).hash_code())
        return 5;

    //Vec3

    if (typeHashCode == typeid(Eigen::Vector3<int>).hash_code())
        return 6;
    if (typeHashCode == typeid(Eigen::Vector3<float>).hash_code())
        return 7;
    if (typeHashCode == typeid(Eigen::Vector3<double>).hash_code())
        return 8;

    //Color

    if (typeHashCode == typeid(Eigen::Vector4<float>).hash_code())
        return 9;

    //Spatial Number

    if (typeHashCode == typeid(Gin::Spatial::Spatial<int>).hash_code())
        return 10;
    if (typeHashCode == typeid(Gin::Spatial::Spatial<float>).hash_code())
        return 11;
    if (typeHashCode == typeid(Gin::Spatial::Spatial<double>).hash_code())
        return 12;

    //Spatial Vec2

    if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector2<int>>).hash_code())
        return 13;
    if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector2<float>>).hash_code())
        return 14;
    if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector2<double>>).hash_code())
        return 15;

    //Spatial Vec3

    if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector3<int>>).hash_code())
        return 16;
    if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector3<float>>).hash_code())
        return 17;
    if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector3<double>>).hash_code())
        return 18;

    //Spatial Color

    if (typeHashCode == typeid(Gin::Spatial::Spatial<Eigen::Vector4<float>>).hash_code())
        return 19;
}

nlohmann::json SerializeGraphPort(Gin::Graph::GraphPort& port) {
	nlohmann::json serializedPort = nlohmann::json::object();

	serializedPort["type"] = GetTypeIdFromHashCode(port.GetType().info.get().hash_code());
	serializedPort["name"] = port.GetName();

	return serializedPort;
}

void DeserializeGraphInputPort(nlohmann::json& serializedPort, Gin::Graph::Graph& graph) {
    uint32_t typeId = serializedPort["type"];

    //Number

    switch (typeId) {
        case 0: {
            graph.AddInput<int>(serializedPort["name"]);
            break;
        };
        case 1: {
            graph.AddInput<float>(serializedPort["name"]);
            break;
        };
        case 2: {
            graph.AddInput<double>(serializedPort["name"]);
            break;
        };
        case 3: {
            graph.AddInput<Eigen::Vector2<int>>(serializedPort["name"]);
            break;
        };
        case 4: {
            graph.AddInput<Eigen::Vector2<float>>(serializedPort["name"]);
            break;
        };
        case 5: {
            graph.AddInput<Eigen::Vector2<double>>(serializedPort["name"]);
            break;
        };
        case 6: {
            graph.AddInput<Eigen::Vector3<int>>(serializedPort["name"]);
            break;
        };
        case 7: {
            graph.AddInput<Eigen::Vector3<float>>(serializedPort["name"]);
            break;
        };
        case 8: {
            graph.AddInput<Eigen::Vector3<double>>(serializedPort["name"]);
            break;
        };
        case 9: {
            graph.AddInput<Eigen::Vector4<float>>(serializedPort["name"]);
            break;
        };
    }
}

void DeserializeGraphOutputPort(nlohmann::json& serializedPort, Gin::Graph::Graph& graph) {
	uint32_t typeId = serializedPort["type"];

	//Number

    switch (typeId) {
        case 0: {
            graph.AddOutput<int>(serializedPort["name"]);
            break;
        };
        case 1: {
            graph.AddOutput<float>(serializedPort["name"]);
            break;
        };
        case 2: {
            graph.AddOutput<double>(serializedPort["name"]);
            break;
        };
        case 3: {
            graph.AddOutput<Eigen::Vector2<int>>(serializedPort["name"]);
            break;
        };
        case 4: {
            graph.AddOutput<Eigen::Vector2<float>>(serializedPort["name"]);
            break;
        };
        case 5: {
            graph.AddOutput<Eigen::Vector2<double>>(serializedPort["name"]);
            break;
        };
        case 6: {
            graph.AddOutput<Eigen::Vector3<int>>(serializedPort["name"]);
            break;
        };
        case 7: {
            graph.AddOutput<Eigen::Vector3<float>>(serializedPort["name"]);
            break;
        };
        case 8: {
            graph.AddOutput<Eigen::Vector3<double>>(serializedPort["name"]);
            break;
        };
        case 9: {
            graph.AddOutput<Eigen::Vector4<float>>(serializedPort["name"]);
            break;
        };
        case 10: {
            graph.AddOutput<Gin::Spatial::Spatial<int>>(serializedPort["name"]);
            break;
        };
        case 11: {
            graph.AddOutput<Gin::Spatial::Spatial<float>>(serializedPort["name"]);
            break;
        };
        case 12: {
            graph.AddOutput<Gin::Spatial::Spatial<double>>(serializedPort["name"]);
            break;
        };
        case 13: {
            graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector2<int>>>(serializedPort["name"]);
            break;
        };
        case 14: {
            graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector2<float>>>(serializedPort["name"]);
            break;
        };
        case 15: {
            graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector2<double>>>(serializedPort["name"]);
            break;
        };
        case 16: {
            graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector3<int>>>(serializedPort["name"]);
            break;
        };
        case 17: {
            graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector3<float>>>(serializedPort["name"]);
            break;
        };
        case 18: {
            graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector3<double>>>(serializedPort["name"]);
            break;
        };
        case 19: {
            graph.AddOutput<Gin::Spatial::Spatial<Eigen::Vector4<float>>>(serializedPort["name"]);
            break;
        };
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

	for (GraphId nodeId = 0; nodeId < graph.GetNodeCount(); ++nodeId) {
		Gin::Graph::GraphNodeOperator<Gin::Graph::Node> node = graph.GetNode<Gin::Graph::Node>(nodeId);

		for (GraphId portId = 0; portId < node->GetInputPortCount(); ++portId) {
			for (std::pair<GraphId, GraphId>& link : adj[nodeId][portId]) {
				serializedGraph.links.push_back(nodeId);
				serializedGraph.links.push_back(portId);
				serializedGraph.links.push_back(link.first);
				serializedGraph.links.push_back(link.second);
			}
		}
	}

	for (GraphId portId = 0; portId < graph.GetOutputsCount(); ++portId) {
		auto& port = graph.GetOutputPort(portId);

		for (std::pair<GraphId, GraphId>& link : port.GetLinks()) {
			serializedGraph.links.push_back(GRAPH_OUTPUT_NODE_ID);
			serializedGraph.links.push_back(portId);
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
            GraphId nodeId = Module::GetNodeRegistry()[nodePath](graph, nodePath);
			graph.GetNode<Node>(nodeId)->Deserialize(serializedGraph.nodesData[i]["node"]);
		} else {
            throw std::runtime_error{ "Missing node, can't deserialize this graph." };
        }
	}

    for (size_t i = 0; i < serializedGraph.graphData["inputs"].size(); ++i)
        DeserializeGraphInputPort(serializedGraph.graphData["inputs"][i], graph);

	for (size_t i = 0; i < serializedGraph.graphData["outputs"].size(); ++i)
		DeserializeGraphOutputPort(serializedGraph.graphData["outputs"][i], graph);

	for (size_t i = 0; i < serializedGraph.links.size(); i += 4) {
		GraphId nodeAId = serializedGraph.links[i];
		GraphId portAId = serializedGraph.links[i + 1];
		GraphId nodeBId = serializedGraph.links[i + 2];
		GraphId portBId = serializedGraph.links[i + 3];

		if (nodeBId == GRAPH_INPUT_NODE_ID) {
			graph.GetNode<Node>(nodeAId).GetPort(portAId).LinkGraphInput(portBId);
		}
		else if (nodeAId == GRAPH_OUTPUT_NODE_ID) {
			graph.GetNode<Node>(nodeBId).GetPort(portBId).LinkGraphOutput(portAId);
		}
		else {
			graph.GetNode<Node>(nodeAId).GetPort(portAId).Link(graph.GetNode<Node>(nodeBId).GetPort(portBId));
		}
	}
}

void Gin::Graph::Serialization::SaveSerializedGraphToFile(SerializedGraph& serializedGraph, std::string path) {
	nlohmann::json jsonGraph{};

	jsonGraph["name"] = serializedGraph.graphName;
	jsonGraph["data"] = serializedGraph.graphData;

	nlohmann::json jsonNodes = nlohmann::json::array();
	
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
	nlohmann::json jsonGraph = nlohmann::json::parse(f);
	f.close();

	serializedGraph.graphName = jsonGraph["name"];
	serializedGraph.graphData = jsonGraph["data"];

	nlohmann::json jsonNodes = jsonGraph["nodes"];
	
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

	nlohmann::json jsonNodes = json["nodes"];

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
