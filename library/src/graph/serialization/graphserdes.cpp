#include "gin/graph/serialization/graphserdes.hpp"
#include <fstream>
#include "gin/module/registry.hpp"
#include <gin/field/sampler.hpp>

size_t GetTypeHashCodeFromId(uint32_t id) {
    switch (id) {
        case 0: return typeid(Gin::Field::Sampler<float>).hash_code();
        case 1: return typeid(Gin::Field::Sampler<Gin::Math::Vector2>).hash_code();
        case 2: return typeid(Gin::Field::Sampler<Gin::Math::Vector3>).hash_code();
        case 3: return typeid(Gin::Field::Sampler<Gin::Math::Vector4>).hash_code();
        case 4: return typeid(Gin::Field::DynamicSampler).hash_code();
        default : return typeid(void).hash_code();
    }
}

uint32_t GetTypeIdFromHashCode(size_t typeHashCode) {
    if (typeHashCode == typeid(Gin::Field::Sampler<float>).hash_code())
        return 0;
    if (typeHashCode == typeid(Gin::Field::Sampler<Gin::Math::Vector2>).hash_code())
        return 1;
    if (typeHashCode == typeid(Gin::Field::Sampler<Gin::Math::Vector3>).hash_code())
        return 2;
    if (typeHashCode == typeid(Gin::Field::Sampler<Gin::Math::Vector4>).hash_code())
        return 3;
    if (typeHashCode == typeid(Gin::Field::DynamicSampler).hash_code())
        return 4;
    return 0;
}

nlohmann::json SerializeGraphPort(Gin::Graph::GraphPort& port, bool input = false) {
	nlohmann::json serializedPort = nlohmann::json::object();

    uint32_t pt = GetTypeIdFromHashCode(port.GetType().info.get().hash_code());;
	serializedPort["type"] = pt;
	serializedPort["name"] = port.GetName();

    if (input) {
        switch (pt) {
            case 0:
            {
                Gin::Field::Sampler<float>* sampler = (Gin::Field::Sampler<float>*)port.GetProperty();
                if (sampler->IsFieldOfType<Gin::Field::ConstantField<float>>()) {
                    serializedPort["value"] = sampler->GetField<Gin::Field::ConstantField<float>>()->Get();
                }
                break;
            }
            default:
                break;
        }
    }

	return serializedPort;
}

void DeserializeGraphInputPort(nlohmann::json& serializedPort, Gin::Graph::Graph& graph) {
    uint32_t typeId = serializedPort["type"];

    //Number

    switch (typeId) {
        case 0: {
            graph.AddInput<Gin::Field::Sampler<float>>(serializedPort["name"]);
            if (serializedPort.count("value")) {
                float v = serializedPort["value"];
                graph.GetInputPort(graph.GetInputsCount() - 1).SetValue<Gin::Field::Sampler<float>>(v);
            }
            break;
        };
        case 1: {
            graph.AddInput<Gin::Field::Sampler<Gin::Math::Vector2>>(serializedPort["name"]);
            break;
        };
        case 2: {
            graph.AddInput<Gin::Field::Sampler<Gin::Math::Vector3>>(serializedPort["name"]);
            break;
        };
        case 3: {
            graph.AddInput<Gin::Field::Sampler<Gin::Math::Vector4>>(serializedPort["name"]);
            break;
        };
    }
}

void DeserializeGraphOutputPort(nlohmann::json& serializedPort, Gin::Graph::Graph& graph) {
	uint32_t typeId = serializedPort["type"];

	//Number

    switch (typeId) {
        case 0: {
            graph.AddOutput<Gin::Field::Sampler<float>>(serializedPort["name"]);
            break;
        };
        case 1: {
            graph.AddOutput<Gin::Field::Sampler<Gin::Math::Vector2>>(serializedPort["name"]);
            break;
        };
        case 2: {
            graph.AddOutput<Gin::Field::Sampler<Gin::Math::Vector3>>(serializedPort["name"]);
            break;
        };
        case 3: {
            graph.AddOutput<Gin::Field::Sampler<Gin::Math::Vector4>>(serializedPort["name"]);
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
		nlohmann::json serializedPort = SerializeGraphPort(graph.GetInputPort(i), true);
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
