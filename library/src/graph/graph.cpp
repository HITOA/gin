#include <gin/graph/graph.hpp>
#include <queue>

void Gin::Graph::GraphPortOperator::Link(GraphPortOperator port) {
	if (port.graph != graph)
		throw std::invalid_argument{ "Can't link port node because it does not belong to the same graph." };

	std::shared_ptr<Node> nodeA = graph->nodes[nodeIdx];
	std::shared_ptr<Node> nodeB = graph->nodes[port.nodeIdx];

	if (portIdx >= nodeA->GetInputPortCount() && port.portIdx >= nodeB->GetInputPortCount())
		throw std::invalid_argument{ "Can't link two output port together." };

	if (portIdx < nodeA->GetInputPortCount() && port.portIdx < nodeB->GetInputPortCount())
		throw std::invalid_argument{ "Can't link two input port together." };

	if (portIdx < nodeA->GetInputPortCount()) {
		if (graph->adj[nodeIdx][portIdx].size() > 0)
			throw std::invalid_argument{ "Input port is already linked to something." };

		//if (nodeA->GetInputPort(portIdx).type != nodeB->GetOutputPort(port.portIdx - nodeB->GetInputPortCount()).type)
		//	throw std::invalid_argument{ "Can't link port that are not the same type" };

		//if (!graph->CheckPortTypeCombination(nodeA->GetInputPort(portIdx).type, nodeB->GetOutputPort(port.portIdx - nodeB->GetInputPortCount()).type))
		//	throw std::invalid_argument{ "Can't link port because of type missmatch" };

		if (!nodeA->GetInputPort(portIdx).Match(nodeB->GetOutputPort(port.portIdx - nodeB->GetInputPortCount())))
			throw std::invalid_argument{ "Can't link port because of type missmatch" };
	}
	else {
		if (graph->adj[port.nodeIdx][port.portIdx].size() > 0)
			throw std::invalid_argument{ "Input port is already linked to something." };

		//if (nodeA->GetOutputPort(portIdx - nodeA->GetInputPortCount()).type != nodeB->GetInputPort(port.portIdx).type)
		//	throw std::invalid_argument{ "Can't link port that are not the same type" };

		//if (!graph->CheckPortTypeCombination(nodeA->GetOutputPort(portIdx - nodeA->GetInputPortCount()).type, nodeB->GetInputPort(port.portIdx).type))
		//	throw std::invalid_argument{ "Can't link port because of type missmatch" };

		if (!nodeA->GetOutputPort(portIdx - nodeA->GetInputPortCount()).Match(nodeB->GetInputPort(port.portIdx)))
			throw std::invalid_argument{ "Can't link port because of type missmatch" };
	}

	graph->adj[nodeIdx][portIdx].emplace_back(port.nodeIdx, port.portIdx);
	graph->adj[port.nodeIdx][port.portIdx].emplace_back(nodeIdx, portIdx);
}

void Gin::Graph::GraphPortOperator::Unlink(GraphPortOperator port)
{
	if (port.graph != graph)
		throw std::invalid_argument{ "Can't unlink port node because it does not belong to the same graph." };

	auto& linkAit = std::find(graph->adj[nodeIdx][portIdx].begin(), graph->adj[nodeIdx][portIdx].end(), std::make_pair(port.nodeIdx, port.portIdx));
	auto& linkBit = std::find(graph->adj[port.nodeIdx][port.portIdx].begin(), graph->adj[port.nodeIdx][port.portIdx].end(), std::make_pair(nodeIdx, portIdx));

	if (linkAit == std::end(graph->adj[nodeIdx][portIdx]) || linkBit == std::end(graph->adj[port.nodeIdx][port.portIdx]))
		return;

	graph->adj[nodeIdx][portIdx].erase(linkAit);
	graph->adj[port.nodeIdx][port.portIdx].erase(linkBit);
}

void  Gin::Graph::GraphPortOperator::LinkGraphInput(size_t idx) {
	std::shared_ptr<Node> node = graph->nodes[nodeIdx];

	if (portIdx >= node->GetInputPortCount())
		throw std::invalid_argument{ "Can't link output port to graph input" };

	if (graph->inputs.size() <= idx)
		throw std::invalid_argument{ "Invalid graph input index" };

	//if (node->GetInputPort(portIdx).type != graph->inputs[idx].type)
	//	throw std::invalid_argument{ "Can't link port that are not the same type" };

	//if (!graph->CheckPortTypeCombination(node->GetInputPort(portIdx).type, graph->inputs[idx].type))
	//	throw std::invalid_argument{ "Can't link port because of type missmatch" };

	if (!node->GetInputPort(portIdx).Match(graph->inputs[idx]))
		throw std::invalid_argument{ "Can't link port because of type missmatch" };


	if (graph->adj[nodeIdx][portIdx].size() > 0)
		throw std::invalid_argument{ "Input port is already linked to something." };

	graph->inputs[idx].GetLinks().emplace_back(nodeIdx, portIdx);
	graph->adj[nodeIdx][portIdx].emplace_back(std::numeric_limits<size_t>::max(), idx);
}

void  Gin::Graph::GraphPortOperator::LinkGraphOutput(size_t idx) {
	std::shared_ptr<Node> node = graph->nodes[nodeIdx];

	if (portIdx < node->GetInputPortCount())
		throw std::invalid_argument{ "Can't link input port to graph output" };

	if (graph->outputs.size() <= idx)
		throw std::invalid_argument{ "Invalid graph input index" };

	//if (node->GetOutputPort(portIdx - node->GetInputPortCount()).type != graph->outputs[idx].type)
	//	throw std::invalid_argument{ "Can't link port that are not the same type" };

	//if (!graph->CheckPortTypeCombination(node->GetOutputPort(portIdx - node->GetInputPortCount()).type, graph->outputs[idx].type))
	//	throw std::invalid_argument{ "Can't link port because of type missmatch" };

	if (!node->GetOutputPort(portIdx - node->GetInputPortCount()).Match(graph->outputs[idx]))
		throw std::invalid_argument{ "Can't link port because of type missmatch" };

	if (graph->outputs[idx].GetLinks().size() > 0)
		throw std::invalid_argument{ "Graph output is already linked to something." };

	graph->outputs[idx].GetLinks().emplace_back(nodeIdx, portIdx);
	graph->adj[nodeIdx][portIdx].emplace_back(std::numeric_limits<size_t>::max() - 1, idx);
}

void Gin::Graph::GraphPortOperator::LinkGraphInput(std::string_view name)
{
	auto& portIt = std::find_if(graph->inputs.begin(), graph->inputs.end(), [&](GraphPort& port) {
		return port.GetName() == name;
	});

	if (portIt == std::end(graph->inputs))
		throw std::invalid_argument{ "No graph output with this name." };

	LinkGraphInput(std::distance(graph->inputs.begin(), portIt));
}

void Gin::Graph::GraphPortOperator::LinkGraphOutput(std::string_view name)
{
	auto& portIt = std::find_if(graph->outputs.begin(), graph->outputs.end(), [&](GraphPort& port) {
		return port.GetName() == name;
		});

	if (portIt == std::end(graph->outputs))
		throw std::invalid_argument{ "No graph output with this name." };

	LinkGraphOutput(std::distance(graph->outputs.begin(), portIt));
}

void Gin::Graph::GraphPortOperator::UnlinkGraphInput(size_t idx)
{
	if (graph->adj[nodeIdx][portIdx].size() < 1)
		return;

	auto& linkGIt = std::find(graph->inputs[idx].GetLinks().begin(), graph->inputs[idx].GetLinks().end(), std::make_pair(nodeIdx, portIdx));
	auto& linkAIt = std::find(graph->adj[nodeIdx][portIdx].begin(), graph->adj[nodeIdx][portIdx].end(), std::make_pair(std::numeric_limits<size_t>::max(), idx));

	if (linkGIt == std::end(graph->inputs[idx].GetLinks()) || linkAIt == std::end(graph->adj[nodeIdx][portIdx]))
		return;

	graph->inputs[idx].GetLinks().erase(linkGIt);
	graph->adj[nodeIdx][portIdx].erase(linkAIt);
}

void Gin::Graph::GraphPortOperator::UnlinkGraphOutput(size_t idx)
{
	if (graph->outputs[idx].GetLinks().size() < 1)
		return;

	auto& linkGIt = std::find(graph->outputs[idx].GetLinks().begin(), graph->outputs[idx].GetLinks().end(), std::make_pair(nodeIdx, portIdx));
	auto& linkAIt = std::find(graph->adj[nodeIdx][portIdx].begin(), graph->adj[nodeIdx][portIdx].end(), std::make_pair(std::numeric_limits<size_t>::max() - 1, idx));

	if (linkGIt == std::end(graph->outputs[idx].GetLinks()) || linkAIt == std::end(graph->adj[nodeIdx][portIdx]))
		return;

	graph->outputs[idx].GetLinks().erase(linkGIt);
	graph->adj[nodeIdx][portIdx].erase(linkAIt);
}

void Gin::Graph::GraphPortOperator::UnlinkGraphInput(std::string_view name)
{
	auto& portIt = std::find_if(graph->inputs.begin(), graph->inputs.end(), [&](GraphPort& port) {
		return port.GetName() == name;
		});

	if (portIt == std::end(graph->inputs))
		throw std::invalid_argument{ "No graph output with this name." };

	UnlinkGraphInput(std::distance(graph->inputs.begin(), portIt));
}

void Gin::Graph::GraphPortOperator::UnlinkGraphOutput(std::string_view name)
{
	auto& portIt = std::find_if(graph->outputs.begin(), graph->outputs.end(), [&](GraphPort& port) {
		return port.GetName() == name;
		});

	if (portIt == std::end(graph->outputs))
		throw std::invalid_argument{ "No graph output with this name." };

	UnlinkGraphOutput(std::distance(graph->outputs.begin(), portIt));
}

void Gin::Graph::Graph::RemoveNode(size_t nodeIdx) {
	nodes.erase(nodes.begin() + nodeIdx);
	adj.erase(adj.begin() + nodeIdx);

	for (auto& nodeadj : adj) {
		for (auto& portadj : nodeadj) {
			for (size_t i = 0; i < portadj.size();) {
				if (portadj[i].first == nodeIdx) {
					portadj.erase(portadj.begin() + i);
					continue;
				}

				if (portadj[i].first > nodeIdx && portadj[i].first < std::numeric_limits<size_t>::max() - 1)
					portadj[i].first -= 1;

				++i;
			}
		}
	}

	for (auto& input : inputs) {
		for (size_t i = 0; i < input.GetLinks().size();) {
			if (input.GetLinks()[i].first == nodeIdx) {
				input.GetLinks().erase(input.GetLinks().begin() + i);
				continue;
			}

			if (input.GetLinks()[i].first > nodeIdx)
				input.GetLinks()[i].first -= 1;

			++i;
		}
	}

	for (auto& output : outputs) {
		for (size_t i = 0; i < output.GetLinks().size();) {
			if (output.GetLinks()[i].first == nodeIdx) {
				output.GetLinks().erase(output.GetLinks().begin() + i);
				continue;
			}

			if (output.GetLinks()[i].first > nodeIdx)
				output.GetLinks()[i].first -= 1;

			++i;
		}
	}
}


void Gin::Graph::Graph::RemoveInput(size_t idx) {
	inputs.erase(inputs.begin() + idx);

	for (auto& nodeadj : adj) {
		for (auto& portadj : nodeadj) {
			for (size_t i = 0; i < portadj.size();) {
				if (portadj[i].first == std::numeric_limits<size_t>::max() && portadj[i].second == idx) {
					portadj.erase(portadj.begin() + i);
					continue;
				}

				if (portadj[i].first == std::numeric_limits<size_t>::max() && portadj[i].second > idx)
					portadj[i].second -= 1;

				++i;
			}
		}
	}
}

void Gin::Graph::Graph::RemoveOutput(size_t idx) {
	outputs.erase(outputs.begin() + idx);

	for (auto& nodeadj : adj) {
		for (auto& portadj : nodeadj) {
			for (size_t i = 0; i < portadj.size();) {
				if (portadj[i].first == std::numeric_limits<size_t>::max() - 1 && portadj[i].second == idx) {
					portadj.erase(portadj.begin() + i);
					continue;
				}

				if (portadj[i].first == std::numeric_limits<size_t>::max() - 1 && portadj[i].second > idx)
					portadj[i].second -= 1;

				++i;
			}
		}
	}
}

void Gin::Graph::Graph::Compile()
{
	program.clear();
;
	std::vector<size_t> nodesOrder{};
	nodesOrder.resize(nodes.size());

	size_t order = 1;

	for (auto& output : outputs) {
		if (output.GetLinks().size() > 0) {
			nodesOrder[output.GetLinks()[0].first] = order;
		}
	}

	size_t count{};
	do {
		count = 0;
		for (size_t nodeIdx = 0; nodeIdx < nodesOrder.size(); ++nodeIdx) {
			if (nodesOrder[nodeIdx] == order) {
				++count;

				for (size_t i = 0; i < nodes[nodeIdx]->GetInputPortCount(); ++i) {
					for (auto& link : adj[nodeIdx][i]) {
						if (link.first < std::numeric_limits<size_t>::max() - 1)
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

	for (size_t nodeIdx = 0; nodeIdx < nodesOrder.size(); ++nodeIdx) {
		bool isolatedNode = true;
		for (size_t i = 0; i < nodes[nodeIdx]->GetInputPortCount(); ++i) {
			if (adj[nodeIdx][i].size() > 0) {
				isolatedNode = false;
				break;
			}
		}
		if (isolatedNode && nodesOrder[nodeIdx] > 0)
			nodesOrder[nodeIdx] = order;
	}

	size_t tmp = order;

	while (order > 0) {
		for (size_t nodeIdx = 0; nodeIdx < nodesOrder.size(); ++nodeIdx) {
			if (nodesOrder[nodeIdx] == order) {
				for (size_t i = 0; i < nodes[nodeIdx]->GetOutputPortCount(); ++i) {
					for (auto& link : adj[nodeIdx][i + nodes[nodeIdx]->GetInputPortCount()]) {
						if (link.first < std::numeric_limits<size_t>::max() - 1)
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
		for (size_t nodeIdx = 0; nodeIdx < nodesOrder.size(); ++nodeIdx) {
			if (nodesOrder[nodeIdx] == order) {
				for (size_t i = 0; i < nodes[nodeIdx]->GetInputPortCount(); ++i) {
					for (auto& link : adj[nodeIdx][i]) {
						GraphAction action{};
						action.type = GraphActionType::COPY;
						action.nodeAIdx = nodeIdx;
						action.portAIdx = i;
						action.nodeBIdx = link.first;
						action.portBIdx = link.second;
						program.push_back(action);
					}
				}
			}
		}

		for (size_t nodeIdx = 0; nodeIdx < nodesOrder.size(); ++nodeIdx) {
			if (nodesOrder[nodeIdx] == order) {
				GraphAction action{};
				action.type = GraphActionType::EXEC;
				action.nodeAIdx = nodeIdx;
				program.push_back(action);
			}
		}
		--order;
	}

	for (size_t i = 0; i < outputs.size(); ++i) {
		if (outputs[i].GetLinks().size() > 0) {
			GraphAction action{};
			action.type = GraphActionType::COPY;
			action.nodeAIdx = std::numeric_limits<size_t>::max() - 1;
			action.portAIdx = i;
			action.nodeBIdx = outputs[i].GetLinks()[0].first;
			action.portBIdx = outputs[i].GetLinks()[0].second;
			program.push_back(action);
		}
	}

	//check for transforming Copy to Move action
}

void Gin::Graph::Graph::Execute(GraphContext ctx)
{
	for (auto& action : program) {
		if (action.type == Gin::Graph::GraphActionType::EXEC)
			nodes[action.nodeAIdx]->Initialize(ctx);
	}

	for (auto& action = program.begin(); action != program.end(); ++action) {
		switch (action->type)
		{
		case Gin::Graph::GraphActionType::EXEC:
			nodes[action->nodeAIdx]->Execute(ctx);
			break;
		case Gin::Graph::GraphActionType::COPY:
			if (action->nodeAIdx == std::numeric_limits<size_t>::max() - 1) {
				//Output to graph output
				Port& src = nodes[action->nodeBIdx]->GetOutputPort(action->portBIdx - nodes[action->nodeBIdx]->GetInputPortCount());
				GraphPort& dst = outputs[action->portAIdx];
				dst.CopyFrom(src);
			}
			else if (action->nodeBIdx == std::numeric_limits<size_t>::max()) {
				//Graph input to input
				GraphPort& src = inputs[action->portBIdx];
				Port& dst = nodes[action->nodeAIdx]->GetInputPort(action->portAIdx);
				dst.CopyFrom(src);
			}
			else {
				Port& src = nodes[action->nodeBIdx]->GetOutputPort(action->portBIdx - nodes[action->nodeBIdx]->GetInputPortCount());
				Port& dst = nodes[action->nodeAIdx]->GetInputPort(action->portAIdx);
				dst.CopyFrom(src);
			}
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
	for (auto& action : program) {
		if (action.type == Gin::Graph::GraphActionType::EXEC)
			nodes[action.nodeAIdx]->Initialize(ctx);
	}

	for (size_t idx = 0; idx < program.size();) {
		GraphAction& action = program[idx];
		switch (action.type)
		{
		case Gin::Graph::GraphActionType::EXEC:
			{
				while (program[idx].type == Gin::Graph::GraphActionType::EXEC) {
					std::shared_ptr<Node> nodeI = nodes[program[idx].nodeAIdx];
					GraphContext ctxI = ctx;
					++running;
					pool.Execute([this, nodeI, ctxI]() {
						nodeI->Execute(ctxI);
						{
							std::unique_lock<std::mutex> lock{ mutex };
							--running;
							cv.notify_all();
						}
					});
					++idx;
				}

				while (running) {
					std::unique_lock<std::mutex> lock{ mutex };
					cv.wait(lock, [&] {
						return running <= 0;
						});
				}

				continue;
			}

			//nodes[action->nodeAIdx]->Initialize(ctx, pool);
			//nodes[action->nodeAIdx]->Execute(ctx, pool);
			break;
		case Gin::Graph::GraphActionType::COPY:
			if (action.nodeAIdx == std::numeric_limits<size_t>::max() - 1) {
				//Output to graph output
				Port& src = nodes[action.nodeBIdx]->GetOutputPort(action.portBIdx - nodes[action.nodeBIdx]->GetInputPortCount());
				GraphPort& dst = outputs[action.portAIdx];
				dst.CopyFrom(src);
			}
			else if (action.nodeBIdx == std::numeric_limits<size_t>::max()) {
				//Graph input to input
				GraphPort& src = inputs[action.portBIdx];
				Port& dst = nodes[action.nodeAIdx]->GetInputPort(action.portAIdx);
				dst.CopyFrom(src);
			}
			else {
				Port& src = nodes[action.nodeBIdx]->GetOutputPort(action.portBIdx - nodes[action.nodeBIdx]->GetInputPortCount());
				Port& dst = nodes[action.nodeAIdx]->GetInputPort(action.portAIdx);
				dst.CopyFrom(src);
			}
			break;
		case Gin::Graph::GraphActionType::MOVE:
			break;
		default:
			break;
		}
		++idx;
	}
}
