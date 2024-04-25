#include <gin/graph/graph.hpp>
#include <queue>
#include <unordered_set>

void Gin::Graph::GraphPortOperator::Link(GraphPortOperator port) {
	if (port.graph != graph)
		throw std::invalid_argument{ "Can't link port node because it does not belong to the same graph." };

	std::shared_ptr<Node> nodeA = graph->nodes[nodeId];
	std::shared_ptr<Node> nodeB = graph->nodes[port.nodeId];

	if (portId >= nodeA->GetInputPortCount() && port.portId >= nodeB->GetInputPortCount())
		throw std::invalid_argument{ "Can't link two output port together." };

	if (portId < nodeA->GetInputPortCount() && port.portId < nodeB->GetInputPortCount())
		throw std::invalid_argument{ "Can't link two input port together." };

	if (portId < nodeA->GetInputPortCount()) {
		if (graph->adj[nodeId][portId].size() > 0)
			throw std::invalid_argument{ "Input port is already linked to something." };

		//if (nodeA->GetInputPort(portId).type != nodeB->GetOutputPort(port.portId - nodeB->GetInputPortCount()).type)
		//	throw std::invalid_argument{ "Can't link port that are not the same type" };

		//if (!graph->CheckPortTypeCombination(nodeA->GetInputPort(portId).type, nodeB->GetOutputPort(port.portId - nodeB->GetInputPortCount()).type))
		//	throw std::invalid_argument{ "Can't link port because of type missmatch" };

		if (!nodeA->GetInputPort(portId).Match(nodeB->GetOutputPort(port.portId - nodeB->GetInputPortCount())))
			throw std::invalid_argument{ "Can't link port because of type missmatch" };
	}
	else {
		if (graph->adj[port.nodeId][port.portId].size() > 0)
			throw std::invalid_argument{ "Input port is already linked to something." };

		//if (nodeA->GetOutputPort(portId - nodeA->GetInputPortCount()).type != nodeB->GetInputPort(port.portId).type)
		//	throw std::invalid_argument{ "Can't link port that are not the same type" };

		//if (!graph->CheckPortTypeCombination(nodeA->GetOutputPort(portId - nodeA->GetInputPortCount()).type, nodeB->GetInputPort(port.portId).type))
		//	throw std::invalid_argument{ "Can't link port because of type missmatch" };

		if (!nodeA->GetOutputPort(portId - nodeA->GetInputPortCount()).Match(nodeB->GetInputPort(port.portId)))
			throw std::invalid_argument{ "Can't link port because of type missmatch" };
	}

	graph->adj[nodeId][portId].emplace_back(port.nodeId, port.portId);
	graph->adj[port.nodeId][port.portId].emplace_back(nodeId, portId);

    nodeA->Update();
    nodeB->Update();
}

void Gin::Graph::GraphPortOperator::Unlink(GraphPortOperator port)
{
	if (port.graph != graph)
		throw std::invalid_argument{ "Can't unlink port node because it does not belong to the same graph." };

	auto linkAit = std::find(graph->adj[nodeId][portId].begin(), graph->adj[nodeId][portId].end(), std::make_pair(port.nodeId, port.portId));
	auto linkBit = std::find(graph->adj[port.nodeId][port.portId].begin(), graph->adj[port.nodeId][port.portId].end(), std::make_pair(nodeId, portId));

	if (linkAit == std::end(graph->adj[nodeId][portId]) || linkBit == std::end(graph->adj[port.nodeId][port.portId]))
		return;

	graph->adj[nodeId][portId].erase(linkAit);
	graph->adj[port.nodeId][port.portId].erase(linkBit);
}

void  Gin::Graph::GraphPortOperator::LinkGraphInput(GraphId id) {
	std::shared_ptr<Node> node = graph->nodes[nodeId];

	if (portId >= node->GetInputPortCount())
		throw std::invalid_argument{ "Can't link output port to graph input" };

	if (graph->inputs.size() <= id)
		throw std::invalid_argument{ "Invalid graph input index" };

	//if (node->GetInputPort(portId).type != graph->inputs[Id].type)
	//	throw std::invalid_argument{ "Can't link port that are not the same type" };

	//if (!graph->CheckPortTypeCombination(node->GetInputPort(portId).type, graph->inputs[Id].type))
	//	throw std::invalid_argument{ "Can't link port because of type missmatch" };

	if (!node->GetInputPort(portId).Match(graph->inputs[id]))
		throw std::invalid_argument{ "Can't link port because of type missmatch" };


	if (graph->adj[nodeId][portId].size() > 0)
		throw std::invalid_argument{ "Input port is already linked to something." };

	graph->inputs[id].GetLinks().emplace_back(nodeId, portId);
	graph->adj[nodeId][portId].emplace_back(GRAPH_INPUT_NODE_ID, id);

    node->Update();
}

void  Gin::Graph::GraphPortOperator::LinkGraphOutput(GraphId id) {
	std::shared_ptr<Node> node = graph->nodes[nodeId];

	if (portId < node->GetInputPortCount())
		throw std::invalid_argument{ "Can't link input port to graph output" };

	if (graph->outputs.size() <= id)
		throw std::invalid_argument{ "Invalid graph output index" };

	//if (node->GetOutputPort(portId - node->GetInputPortCount()).type != graph->outputs[Id].type)
	//	throw std::invalid_argument{ "Can't link port that are not the same type" };

	//if (!graph->CheckPortTypeCombination(node->GetOutputPort(portId - node->GetInputPortCount()).type, graph->outputs[Id].type))
	//	throw std::invalid_argument{ "Can't link port because of type missmatch" };

	if (!node->GetOutputPort(portId - node->GetInputPortCount()).Match(graph->outputs[id]))
		throw std::invalid_argument{ "Can't link port because of type missmatch" };

	if (graph->outputs[id].GetLinks().size() > 0)
		throw std::invalid_argument{ "Graph output is already linked to something." };

	graph->outputs[id].GetLinks().emplace_back(nodeId, portId);
	graph->adj[nodeId][portId].emplace_back(GRAPH_OUTPUT_NODE_ID, id);

    node->Update();
}

void Gin::Graph::GraphPortOperator::LinkGraphInput(std::string_view name)
{
	auto portIt = std::find_if(graph->inputs.begin(), graph->inputs.end(), [&](GraphPort& port) {
		return port.GetName() == name;
	});

	if (portIt == std::end(graph->inputs))
		throw std::invalid_argument{ "No graph output with this name." };

	LinkGraphInput(std::distance(graph->inputs.begin(), portIt));
}

void Gin::Graph::GraphPortOperator::LinkGraphOutput(std::string_view name)
{
	auto portIt = std::find_if(graph->outputs.begin(), graph->outputs.end(), [&](GraphPort& port) {
		return port.GetName() == name;
		});

	if (portIt == std::end(graph->outputs))
		throw std::invalid_argument{ "No graph output with this name." };

	LinkGraphOutput(std::distance(graph->outputs.begin(), portIt));
}

void Gin::Graph::GraphPortOperator::UnlinkGraphInput(GraphId id)
{
	if (graph->adj[nodeId][portId].size() < 1)
		return;

	auto linkGIt = std::find(graph->inputs[id].GetLinks().begin(), graph->inputs[id].GetLinks().end(), std::make_pair(nodeId, portId));
	auto linkAIt = std::find(graph->adj[nodeId][portId].begin(), graph->adj[nodeId][portId].end(), std::make_pair(GRAPH_INPUT_NODE_ID, id));

	if (linkGIt == std::end(graph->inputs[id].GetLinks()) || linkAIt == std::end(graph->adj[nodeId][portId]))
		return;

	graph->inputs[id].GetLinks().erase(linkGIt);
	graph->adj[nodeId][portId].erase(linkAIt);
}

void Gin::Graph::GraphPortOperator::UnlinkGraphOutput(GraphId id)
{
	if (graph->outputs[id].GetLinks().size() < 1)
		return;

	auto linkGIt = std::find(graph->outputs[id].GetLinks().begin(), graph->outputs[id].GetLinks().end(), std::make_pair(nodeId, portId));
	auto linkAIt = std::find(graph->adj[nodeId][portId].begin(), graph->adj[nodeId][portId].end(), std::make_pair(GRAPH_OUTPUT_NODE_ID, id));

	if (linkGIt == std::end(graph->outputs[id].GetLinks()) || linkAIt == std::end(graph->adj[nodeId][portId]))
		return;

	graph->outputs[id].GetLinks().erase(linkGIt);
	graph->adj[nodeId][portId].erase(linkAIt);
}

void Gin::Graph::GraphPortOperator::UnlinkGraphInput(std::string_view name)
{
	auto portIt = std::find_if(graph->inputs.begin(), graph->inputs.end(), [&](GraphPort& port) {
		return port.GetName() == name;
		});

	if (portIt == std::end(graph->inputs))
		throw std::invalid_argument{ "No graph output with this name." };

	UnlinkGraphInput(std::distance(graph->inputs.begin(), portIt));
}

void Gin::Graph::GraphPortOperator::UnlinkGraphOutput(std::string_view name)
{
	auto portIt = std::find_if(graph->outputs.begin(), graph->outputs.end(), [&](GraphPort& port) {
		return port.GetName() == name;
		});

	if (portIt == std::end(graph->outputs))
		throw std::invalid_argument{ "No graph output with this name." };

	UnlinkGraphOutput(std::distance(graph->outputs.begin(), portIt));
}

void Gin::Graph::Graph::RemoveNode(GraphId nodeId) {
	nodes.erase(nodes.begin() + nodeId);
	adj.erase(adj.begin() + nodeId);

	for (auto& nodeadj : adj) {
		for (auto& portadj : nodeadj) {
			for (size_t i = 0; i < portadj.size();) {
				if (portadj[i].first == nodeId) {
					portadj.erase(portadj.begin() + i);
					continue;
				}

				if (portadj[i].first > nodeId && portadj[i].first < GRAPH_OUTPUT_NODE_ID)
					portadj[i].first -= 1;

				++i;
			}
		}
	}

	for (auto& input : inputs) {
		for (size_t i = 0; i < input.GetLinks().size();) {
			if (input.GetLinks()[i].first == nodeId) {
				input.GetLinks().erase(input.GetLinks().begin() + i);
				continue;
			}

			if (input.GetLinks()[i].first > nodeId)
				input.GetLinks()[i].first -= 1;

			++i;
		}
	}

	for (auto& output : outputs) {
		for (size_t i = 0; i < output.GetLinks().size();) {
			if (output.GetLinks()[i].first == nodeId) {
				output.GetLinks().erase(output.GetLinks().begin() + i);
				continue;
			}

			if (output.GetLinks()[i].first > nodeId)
				output.GetLinks()[i].first -= 1;

			++i;
		}
	}
}


void Gin::Graph::Graph::RemoveInput(GraphId Id) {
	inputs.erase(inputs.begin() + Id);

	for (auto& nodeadj : adj) {
		for (auto& portadj : nodeadj) {
			for (size_t i = 0; i < portadj.size();) {
				if (portadj[i].first == GRAPH_INPUT_NODE_ID && portadj[i].second == Id) {
					portadj.erase(portadj.begin() + i);
					continue;
				}

				if (portadj[i].first == GRAPH_INPUT_NODE_ID && portadj[i].second > Id)
					portadj[i].second -= 1;

				++i;
			}
		}
	}
}

void Gin::Graph::Graph::RemoveOutput(GraphId Id) {
	outputs.erase(outputs.begin() + Id);

	for (auto& nodeadj : adj) {
		for (auto& portadj : nodeadj) {
			for (size_t i = 0; i < portadj.size();) {
				if (portadj[i].first == GRAPH_OUTPUT_NODE_ID && portadj[i].second == Id) {
					portadj.erase(portadj.begin() + i);
					continue;
				}

				if (portadj[i].first == GRAPH_OUTPUT_NODE_ID && portadj[i].second > Id)
					portadj[i].second -= 1;

				++i;
			}
		}
	}
}

void Gin::Graph::Graph::Compile()
{
	program.clear();

	std::vector<size_t> nodesOrder{};
	nodesOrder.resize(nodes.size());

    std::unordered_set<GraphId> freedNodes{};

	size_t order = 1;

	for (auto& output : outputs) {
		if (output.GetLinks().size() > 0) {
			nodesOrder[output.GetLinks()[0].first] = order;
		}
	}

	size_t count{};
	do {
		count = 0;
		for (size_t nodeId = 0; nodeId < nodesOrder.size(); ++nodeId) {
			if (nodesOrder[nodeId] == order) {
				++count;

				for (size_t i = 0; i < nodes[nodeId]->GetInputPortCount(); ++i) {
					for (auto& link : adj[nodeId][i]) {
						if (link.first < GRAPH_OUTPUT_NODE_ID)
							nodesOrder[link.first] = order + 1;
					}
				}
			}
		}
		++order;
		if (order >= MAX_ORDER_ITERATION_STOP)
			throw std::runtime_error{ "There is a circular dependency in the graph." };
	} while (count);

	order -= 2;

	//Maximize Node Order

	for (auto& input : inputs) {
		for (auto& link : input.GetLinks()) {
			if (nodesOrder[link.first] > 0)
				nodesOrder[link.first] = order;
		}
	}

	for (size_t nodeId = 0; nodeId < nodesOrder.size(); ++nodeId) {
		bool isolatedNode = true;
		for (size_t i = 0; i < nodes[nodeId]->GetInputPortCount(); ++i) {
			if (adj[nodeId][i].size() > 0) {
				isolatedNode = false;
				break;
			}
		}
		if (isolatedNode && nodesOrder[nodeId] > 0)
			nodesOrder[nodeId] = order;
	}

	size_t tmp = order;

	while (order > 0) {
		for (size_t nodeId = 0; nodeId < nodesOrder.size(); ++nodeId) {
			if (nodesOrder[nodeId] == order) {
				for (size_t i = 0; i < nodes[nodeId]->GetOutputPortCount(); ++i) {
					for (auto& link : adj[nodeId][i + nodes[nodeId]->GetInputPortCount()]) {
						if (link.first < GRAPH_OUTPUT_NODE_ID)
							if (nodesOrder[link.first] > 0)
								nodesOrder[link.first] = order - 1;
					}
				}
			}
		}
		--order;
	}

	order = tmp;

	while (order > 0) {
		for (size_t nodeId = 0; nodeId < nodesOrder.size(); ++nodeId) {
			if (nodesOrder[nodeId] == order) {
				for (size_t i = 0; i < nodes[nodeId]->GetInputPortCount(); ++i) {
					for (auto& link : adj[nodeId][i]) {
						GraphAction action{};
						action.type = GraphActionType::COPY;
						action.nodeAId = nodeId;
						action.portAId = i;
						action.nodeBId = link.first;
						action.portBId = link.second;
						program.push_back(action);
					}
				}
			}
		}

		for (size_t nodeId = 0; nodeId < nodesOrder.size(); ++nodeId) {
			if (nodesOrder[nodeId] == order) {
				GraphAction action{};
				action.type = GraphActionType::EXEC;
				action.nodeAId = nodeId;
				program.push_back(action);
			}
		}
		--order;
	}

	for (size_t i = 0; i < outputs.size(); ++i) {
		if (outputs[i].GetLinks().size() > 0) {
			GraphAction action{};
			action.type = GraphActionType::COPY;
			action.nodeAId = GRAPH_OUTPUT_NODE_ID;
			action.portAId = i;
			action.nodeBId = outputs[i].GetLinks()[0].first;
			action.portBId = outputs[i].GetLinks()[0].second;
			program.push_back(action);
		}
	}

    //Insert free operation
    for (int i = program.size() - 1; i >= 0; --i) {
        if (program[i].type == GraphActionType::COPY) {
            if (!freedNodes.count(program[i].nodeBId)) {
                if (program[i].nodeBId == GRAPH_INPUT_NODE_ID)
                    continue;
                GraphAction action{};
                action.type = GraphActionType::FREE;
                action.nodeAId = program[i].nodeBId;
                program.insert(program.begin() + i + 1, action);
                freedNodes.insert(program[i].nodeBId);
            }
        }
    }
}

void Gin::Graph::Graph::Execute(GraphContext ctx)
{
	for (auto action = program.begin(); action != program.end(); ++action) {
		switch (action->type)
		{
		case Gin::Graph::GraphActionType::EXEC:
            nodes[action->nodeAId]->Initialize(ctx);
			nodes[action->nodeAId]->Execute(ctx);
			break;
		case Gin::Graph::GraphActionType::COPY:
			if (action->nodeAId == GRAPH_OUTPUT_NODE_ID) {
				//Output to graph output
				Port& src = nodes[action->nodeBId]->GetOutputPort(action->portBId - nodes[action->nodeBId]->GetInputPortCount());
				GraphPort& dst = outputs[action->portAId];
				dst.CopyFrom(src);
			}
			else if (action->nodeBId == GRAPH_INPUT_NODE_ID) {
				//Graph input to input
				GraphPort& src = inputs[action->portBId];
				Port& dst = nodes[action->nodeAId]->GetInputPort(action->portAId);
				dst.CopyFrom(src);
			}
			else {
				Port& src = nodes[action->nodeBId]->GetOutputPort(action->portBId - nodes[action->nodeBId]->GetInputPortCount());
				Port& dst = nodes[action->nodeAId]->GetInputPort(action->portAId);
				dst.CopyFrom(src);
			}
			break;
        case Gin::Graph::GraphActionType::FREE:
            for (size_t i = 0; i < nodes[action->nodeAId]->GetInputPortCount(); ++i)
                if (!adj[action->nodeAId][i].empty())
                    nodes[action->nodeAId]->GetInputPort(i).Clear();
                for (size_t i = 0; i < nodes[action->nodeAId]->GetOutputPortCount(); ++i)
                    nodes[action->nodeAId]->GetOutputPort(i).Clear();
            break;
		case Gin::Graph::GraphActionType::MOVE:
			break;
		default:
			break;
		}
	}
}


void Gin::Graph::Graph::Execute(GraphContext ctx, Thread::ThreadPool& pool)
{
	for (size_t Id = 0; Id < program.size();) {
		GraphAction& action = program[Id];
		switch (action.type)
		{
		case Gin::Graph::GraphActionType::EXEC:
			{
				while (program[Id].type == Gin::Graph::GraphActionType::EXEC) {
					std::shared_ptr<Node> nodeI = nodes[program[Id].nodeAId];
					GraphContext ctxI = ctx;
					++running;
					pool.Execute([this, nodeI, ctxI, &pool]() {
                        nodeI->Initialize(ctxI);
						nodeI->Execute(ctxI, pool);
						{
							std::unique_lock<std::mutex> lock{ mutex };
							--running;
							cv.notify_all();
						}
					});
					++Id;
				}

				while (running) {
					std::unique_lock<std::mutex> lock{ mutex };
					cv.wait(lock, [&] {
						return running <= 0;
						});
				}

				continue;
			}
			break;
		case Gin::Graph::GraphActionType::COPY:
			if (action.nodeAId == GRAPH_OUTPUT_NODE_ID) {
				//Output to graph output
				Port& src = nodes[action.nodeBId]->GetOutputPort(action.portBId - nodes[action.nodeBId]->GetInputPortCount());
				GraphPort& dst = outputs[action.portAId];
				dst.CopyFrom(src);
			}
			else if (action.nodeBId == GRAPH_INPUT_NODE_ID) {
				//Graph input to input
				GraphPort& src = inputs[action.portBId];
				Port& dst = nodes[action.nodeAId]->GetInputPort(action.portAId);
				dst.CopyFrom(src);
			}
			else {
				Port& src = nodes[action.nodeBId]->GetOutputPort(action.portBId - nodes[action.nodeBId]->GetInputPortCount());
				Port& dst = nodes[action.nodeAId]->GetInputPort(action.portAId);
				dst.CopyFrom(src);
			}
			break;
        case Gin::Graph::GraphActionType::FREE:
            for (size_t i = 0; i < nodes[action.nodeAId]->GetInputPortCount(); ++i)
                if (!adj[action.nodeAId][i].empty())
                    nodes[action.nodeAId]->GetInputPort(i).Clear();
            for (size_t i = 0; i < nodes[action.nodeAId]->GetOutputPortCount(); ++i)
                nodes[action.nodeAId]->GetOutputPort(i).Clear();
            break;
		case Gin::Graph::GraphActionType::MOVE:
			break;
		default:
			break;
		}
		++Id;
	}
}
