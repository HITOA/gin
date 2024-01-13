#pragma once

#include <gin/graph/node.hpp>
#include <gin/graph/graphport.hpp>
#include <gin/graph/graphcontext.hpp>
#include <gin/thread/threadpool.hpp>

#include <vector>
#include <memory>
#include <any>
#include <string>
#include <stdexcept>
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
		EXEC
	};

	struct GraphAction {
		GraphActionType type;
		size_t nodeAIdx;
		size_t portAIdx;
		size_t nodeBIdx;
		size_t portBIdx;
	};

	class Graph {
	public:
		template<typename T, typename... Args>
		inline GraphNodeOperator<T> AddNode(Args... args) {
			size_t c = nodes.size();

			std::shared_ptr<Node> node{ std::dynamic_pointer_cast<Node, T>(std::make_shared<T>(std::forward(args)...)) };

			nodes.push_back(node);

			adj.emplace_back();

			for (size_t i = 0; i < node->GetInputPortCount() + node->GetOutputPortCount(); ++i)
				adj[c].emplace_back();

			return GetNode<T>(c);
		}

		template<typename T>
		inline GraphNodeOperator<T> AddNode(std::shared_ptr<T> node) {
			size_t c = nodes.size();
			nodes.push_back(std::dynamic_pointer_cast<Node, T>(node));

			adj.emplace_back();

			for (size_t i = 0; i < node->GetInputPortCount() + node->GetOutputPortCount(); ++i)
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
		inline void SetInput(size_t idx, T value) {
			inputs[idx].SetValue<T>(value);
		}

		template<typename T>
		inline void SetOutput(size_t idx, T value) {
			outputs[idx].SetValue<T>(value);
		}

		template<typename T>
		inline T GetInput(size_t idx) {
			return *((T*)inputs[idx].GetProperty());
		}

		template<typename T>
		inline T GetOutput(size_t idx) {
			return *((T*)outputs[idx].GetProperty());
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

		inline size_t HasInput(std::string_view name) {
			auto it = std::find_if(inputs.begin(), inputs.end(), [&](GraphPort& port) {
				return name == port.GetName();
				});

			if (it == std::end(inputs))
				return std::numeric_limits<size_t>::max();

			return it - inputs.begin();
		}

		inline size_t HasOutput(std::string_view name) {
			auto it = std::find_if(outputs.begin(), outputs.end(), [&](GraphPort& port) {
				return name == port.GetName();
				});

			if (it == std::end(outputs))
				return std::numeric_limits<size_t>::max();

			return it - outputs.begin();
		}

		template<typename T>
		inline GraphNodeOperator<T> GetNode(size_t nodeIdx) {
			return GraphNodeOperator<T>{ this, nodeIdx };
		}

		inline size_t GetNodeCount() { return nodes.size(); };
		inline size_t GetInputsCount() { return inputs.size(); };
		inline size_t GetOutputsCount() { return outputs.size(); };
		inline GraphPort& GetInputPort(size_t idx) { return inputs[idx]; };
		inline GraphPort& GetOutputPort(size_t idx) { return outputs[idx]; };
		inline std::vector<GraphAction>& GetProgram() { return program; };
		inline std::vector<std::vector<std::vector<std::pair<size_t, size_t>>>>& GetAdj() { return adj; };

		void RemoveNode(size_t nodeIdx);
		void RemoveInput(size_t idx);
		void RemoveOutput(size_t idx);


		void Compile();
		void Execute(GraphContext ctx);
		void Execute(GraphContext ctx, Thread::ThreadPool& pool);

	private:
		std::vector<std::shared_ptr<Node>> nodes{};
		std::vector<std::vector<std::vector<std::pair<size_t, size_t>>>> adj{};

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
		GraphPortOperator(Graph* graph, size_t nodeIdx, size_t portIdx) : graph{ graph }, nodeIdx{ nodeIdx }, portIdx{ portIdx } {};

		inline size_t GetPortIdx() { return portIdx; };

		void Link(GraphPortOperator port);
		void Unlink(GraphPortOperator port);

		void LinkGraphInput(size_t idx);
		void LinkGraphOutput(size_t idx);
		void LinkGraphInput(std::string_view name);
		void LinkGraphOutput(std::string_view name);

		void UnlinkGraphInput(size_t idx);
		void UnlinkGraphOutput(size_t idx);
		void UnlinkGraphInput(std::string_view name);
		void UnlinkGraphOutput(std::string_view name);

	private:
		Graph* graph;
		size_t nodeIdx;
		size_t portIdx;
	};

	template<typename T>
	class GraphNodeOperator {
	public:
		GraphNodeOperator() = delete;
		GraphNodeOperator(Graph* graph, size_t nodeIdx) : graph{ graph }, nodeIdx{ nodeIdx } {};

		inline size_t GetNodeIdx() { return nodeIdx; };

		template<typename U>
		inline GraphPortOperator GetPort(U& property) {
			auto& node = graph->nodes[nodeIdx];

			for (size_t i = 0; i < node->GetInputPortCount(); ++i)
				if (node->GetInputPort(i).GetProperty() == &property)
					return GraphPortOperator { graph, nodeIdx, i};

			for (size_t i = 0; i < node->GetOutputPortCount(); ++i)
				if (node->GetOutputPort(i).GetProperty() == &property)
					return GraphPortOperator { graph, nodeIdx, i + node->GetInputPortCount()};

			throw std::invalid_argument{ "This property does not have any input/output port." };
		}

		inline GraphPortOperator GetPort(size_t portIdx) {
			return GraphPortOperator{ graph, nodeIdx, portIdx };
		}

		inline GraphPortOperator operator()(size_t portIdx) {
			return GraphPortOperator{ graph, nodeIdx, portIdx };
		}

		inline T* operator->() {
			return std::dynamic_pointer_cast<T, Node>(graph->nodes[nodeIdx]).get();
		}
	private:
		Graph* graph;
		size_t nodeIdx;
	};

}