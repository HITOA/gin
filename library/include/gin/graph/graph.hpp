#pragma once

#include <gin/graph/node.hpp>
#include <gin/graph/graphport.hpp>
#include <gin/graph/graphcontext.hpp>
#include <gin/thread/threadpool.hpp>
#include <gin/graph/graphdef.hpp>

#include <vector>
#include <memory>
#include <any>
#include <string>
#include <stdexcept>
#include <cstddef>
#include <unordered_map>
#include <functional>
#include <condition_variable>

#define MAX_ORDER_ITERATION_STOP 1000

namespace Gin::Graph {

	template<typename T>
	class GraphNodeOperator;

	enum class GraphActionType {
		NONE,
		COPY,
		MOVE,
		EXEC,
        FREE
	};

	struct GraphAction {
		GraphActionType type;
        GraphId nodeAId;
        GraphId portAId;
        GraphId nodeBId;
        GraphId portBId;
	};

	class Graph {
	public:
		template<typename T, typename... Args>
		inline GraphNodeOperator<T> AddNode(Args... args) {
			uint32_t c = nodes.size();

			std::shared_ptr<Node> node{ std::dynamic_pointer_cast<Node, T>(std::make_shared<T>(std::forward(args)...)) };

			nodes.push_back(node);

			adj.emplace_back();

			for (uint32_t i = 0; i < node->GetInputPortCount() + node->GetOutputPortCount(); ++i)
				adj[c].emplace_back();

			return GetNode<T>(c);
		}

		template<typename T>
		inline GraphNodeOperator<T> AddNode(std::shared_ptr<T> node) {
            uint32_t c = nodes.size();
			nodes.push_back(std::dynamic_pointer_cast<Node, T>(node));

			adj.emplace_back();

			for (uint32_t i = 0; i < node->GetInputPortCount() + node->GetOutputPortCount(); ++i)
				adj[c].emplace_back();

			return GetNode<T>(c);
		}

		template<typename T>
		inline void AddInput(const std::string& name) {
			GraphPort port{ name };
			port.SetValue<T>(T{});
			inputs.push_back(port);
		}

		template<typename T>
		inline void AddOutput(const std::string& name) {
			GraphPort port{ name };
			port.SetValue<T>(T{});
			outputs.push_back(port);
		}

		template<typename T>
		inline void SetInput(GraphId id, T value) {
			inputs[id].SetValue<T>(value);
		}

		template<typename T>
		inline void SetOutput(GraphId id, T value) {
			outputs[id].SetValue<T>(value);
		}

		template<typename T>
		inline T GetInput(GraphId id) {
			return *((T*)inputs[id].GetProperty());
		}

		template<typename T>
		inline T GetOutput(GraphId id) {
			return *((T*)outputs[id].GetProperty());
		}

		template<typename T>
		inline void SetInput(std::string_view name, T value) {
			auto it = std::find_if(inputs.begin(), inputs.end(), [&](GraphPort& port) {
				return name == port.GetName();
			});

			if (it == std::end(inputs))
				throw std::invalid_argument{ "Bad name." };

			SetInput<T>(std::distance(inputs.begin(), it), value);
		}

		template<typename T>
		inline void SetOutput(std::string_view name, T value) {
			auto it = std::find_if(outputs.begin(), outputs.end(), [&](GraphPort& port) {
				return name == port.GetName();
				});

			if (it == std::end(outputs))
				throw std::invalid_argument{ "Bad name." };

			SetOutput<T>(std::distance(outputs.begin(), it), value);
		}

		template<typename T>
		inline T GetInput(std::string_view name) {
			auto it = std::find_if(inputs.begin(), inputs.end(), [&](GraphPort& port) {
				return name == port.GetName();
				});

			if (it == std::end(inputs))
				throw std::invalid_argument{ "Bad name." };

			return GetInput<T>(std::distance(inputs.begin(), it));
		}

		template<typename T>
		inline T GetOutput(std::string_view name) {
			auto it = std::find_if(outputs.begin(), outputs.end(), [&](GraphPort& port) {
				return name == port.GetName();
				});

			if (it == std::end(outputs))
				throw std::invalid_argument{ "Bad name." };

			return GetOutput<T>(std::distance(outputs.begin(), it));
		}

		inline GraphId HasInput(std::string_view name) {
			auto it = std::find_if(inputs.begin(), inputs.end(), [&](GraphPort& port) {
				return name == port.GetName();
				});

			if (it == std::end(inputs))
				return GRAPH_ID_MAX;

			return it - inputs.begin();
		}

		inline GraphId HasOutput(std::string_view name) {
			auto it = std::find_if(outputs.begin(), outputs.end(), [&](GraphPort& port) {
				return name == port.GetName();
				});

			if (it == std::end(outputs))
				return GRAPH_ID_MAX;

			return it - outputs.begin();
		}

		template<typename T>
		inline GraphNodeOperator<T> GetNode(GraphId nodeId) {
			return GraphNodeOperator<T>{ this, nodeId };
		}

		inline uint32_t GetNodeCount() { return nodes.size(); };
		inline uint32_t GetInputsCount() { return inputs.size(); };
		inline uint32_t GetOutputsCount() { return outputs.size(); };
		inline GraphPort& GetInputPort(GraphId id) { return inputs[id]; };
		inline GraphPort& GetOutputPort(GraphId id) { return outputs[id]; };
		inline std::vector<GraphAction>& GetProgram() { return program; };
		inline std::vector<std::vector<std::vector<std::pair<GraphId, GraphId>>>>& GetAdj() { return adj; };

		void RemoveNode(GraphId nodeId);
		void RemoveInput(GraphId id);
		void RemoveOutput(GraphId id);


		void Compile();
		void Execute(GraphContext ctx);
		void Execute(GraphContext ctx, Thread::ThreadPool& pool);

	private:
		std::vector<std::shared_ptr<Node>> nodes{};
		std::vector<std::vector<std::vector<std::pair<GraphId, GraphId>>>> adj{};

		std::vector<GraphPort> inputs{};
		std::vector<GraphPort> outputs{};

		std::vector<GraphAction> program{};

		unsigned int running{ 0 };
		std::mutex mutex{};
		std::condition_variable cv{};

		friend class GraphPortOperator;
		template<typename T>
		friend class GraphNodeOperator;
	};

	class GraphPortOperator {
	public:
		GraphPortOperator() = delete;
		GraphPortOperator(Graph* graph, GraphId nodeId, GraphId portId) : graph{ graph }, nodeId{ nodeId }, portId{ portId } {};

		inline GraphId GetPortId() { return portId; };

		void Link(GraphPortOperator port);
		void Unlink(GraphPortOperator port);

		void LinkGraphInput(GraphId id);
		void LinkGraphOutput(GraphId id);
		void LinkGraphInput(std::string_view name);
		void LinkGraphOutput(std::string_view name);

		void UnlinkGraphInput(GraphId id);
		void UnlinkGraphOutput(GraphId id);
		void UnlinkGraphInput(std::string_view name);
		void UnlinkGraphOutput(std::string_view name);

	private:
		Graph* graph;
        GraphId nodeId;
        GraphId portId;
	};

	template<typename T>
	class GraphNodeOperator {
	public:
		GraphNodeOperator() = delete;
		GraphNodeOperator(Graph* graph, GraphId nodeId) : graph{ graph }, nodeId{ nodeId } {};

		inline GraphId GetNodeId() { return nodeId; };

		template<typename U>
		inline GraphPortOperator GetPort(U& property) {
			auto& node = graph->nodes[nodeId];

			for (GraphId i = 0; i < node->GetInputPortCount(); ++i)
				if (node->GetInputPort(i).GetProperty() == &property)
					return GraphPortOperator { graph, nodeId, i};

			for (GraphId i = 0; i < node->GetOutputPortCount(); ++i)
				if (node->GetOutputPort(i).GetProperty() == &property)
					return GraphPortOperator { graph, nodeId, i + node->GetInputPortCount()};

			throw std::invalid_argument{ "This property does not have any input/output port." };
		}

		inline GraphPortOperator GetPort(GraphId portId) {
			return GraphPortOperator{ graph, nodeId, portId };
		}

		inline GraphPortOperator operator()(GraphId portId) {
			return GraphPortOperator{ graph, nodeId, portId };
		}

		inline T* operator->() {
			return std::dynamic_pointer_cast<T, Node>(graph->nodes[nodeId]).get();
		}
	private:
		Graph* graph;
        GraphId nodeId;
	};

}