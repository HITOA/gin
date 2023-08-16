#pragma once

#include <gin/graph/port.hpp>
#include <gin/graph/nodeport.hpp>
#include <gin/graph/graphcontext.hpp>
#include <gin/spatial/spatial.hpp>
#include <gin/math/math.hpp>
#include <gin/thread/threadpool.hpp>

#define MAX_INPUT_PORT 32
#define MAX_OUTPUT_PORT 32
#define MAX_PORT (MAX_INPUT_PORT + MAX_OUTPUT_PORT)

namespace Gin::Graph {
	class Node {
	public:
		virtual void Initialize(GraphContext ctx);
		virtual void Initialize(GraphContext ctx, Thread::ThreadPool& pool);
		virtual void Execute(GraphContext ctx) = 0;
		virtual void Execute(GraphContext ctx, Thread::ThreadPool& pool);
		virtual std::string GetName() = 0;

		size_t AddInputPort(std::shared_ptr<Port> port);
		Port& GetInputPort(size_t idx);
		size_t GetInputPortCount();

		size_t AddOutputPort(std::shared_ptr<Port> port);
		Port& GetOutputPort(size_t idx);
		size_t GetOutputPortCount();

		template<typename T>
		inline size_t AddInputPort(const char* name, T& property) {
			return AddInputPort(CreateNodePort(name, property));
		}

		template<typename T>
		inline size_t AddOutputPort(const char* name, T& property) {
			return AddOutputPort(CreateNodePort(name, property));
		}

		template<typename F>
		inline void SpatialOperation(F&& f) {
			Eigen::Vector3<double> t{ ctx.bounds.extent * 2.0 / ctx.scale };
			Eigen::Vector3<int> size = Math::Ceil<double, int, 3>(t);

			size_t idx = 0;
			for (size_t z = 0; z < size.z(); ++z) {
				for (size_t y = 0; y < size.y(); ++y) {
					for (size_t x = 0; x < size.x(); ++x) {
						f(idx, x, y, z);
						++idx;
					}
				}
			}
		}
	private:
		std::vector<std::shared_ptr<Port>> inputs{};
		std::vector<std::shared_ptr<Port>> outputs{};
		GraphContext ctx{};
	};
}