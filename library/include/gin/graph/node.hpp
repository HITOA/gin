#pragma once

#include <gin/graph/port.hpp>
#include <gin/graph/nodeport.hpp>
#include <gin/graph/graphcontext.hpp>
#include <gin/math/math.hpp>
#include <gin/thread/threadpool.hpp>
#include <gin/graph/serialization/json.hpp>
#include <gin/thread/parallel.hpp>

#define MAX_INPUT_PORT 32
#define MAX_OUTPUT_PORT 32
#define MAX_PORT (MAX_INPUT_PORT + MAX_OUTPUT_PORT)

namespace Gin::Graph {
	class Node {
	public:
		virtual void Initialize(GraphContext ctx);
		virtual void Initialize(GraphContext ctx, Thread::ThreadPool& pool);
        virtual void Clear();
        virtual void Update();
		virtual void Execute(GraphContext ctx) = 0;
		virtual void Execute(GraphContext ctx, Thread::ThreadPool& pool);
		virtual std::string GetName() = 0;
		virtual nlohmann::json Serialize();
		virtual void Deserialize(nlohmann::json data);

		size_t AddInputPort(std::shared_ptr<Port> port);
		Port& GetInputPort(size_t idx);
		size_t GetInputPortCount();

		size_t AddOutputPort(std::shared_ptr<Port> port);
		Port& GetOutputPort(size_t idx);
		size_t GetOutputPortCount();

		void SetPath(const std::string& path);
		std::string GetPath();

		template<typename T>
		inline size_t AddInputPort(const char* name, T& property) {
			return AddInputPort(CreateNodePort(name, property));
		}

		template<typename T>
		inline size_t AddOutputPort(const char* name, T& property) {
			return AddOutputPort(CreateNodePort(name, property));
		}

	private:
		std::vector<std::shared_ptr<Port>> inputs{};
		std::vector<std::shared_ptr<Port>> outputs{};
		std::string path{};
	};
}