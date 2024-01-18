#pragma once

#include <gin/graph/node.hpp>

namespace Gin::Module::Signed {

	/**
	 * Union Operation For 2 Signed Distance.
	 */
	class OPUnion : public Graph::Node {
	public:
		OPUnion();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> distanceA{};
		Spatial::Spatial<float> distanceB{};

		Spatial::Spatial<float> distanceR{};
		Spatial::Spatial<float> t{};
	};

	/**
	 * Substraction Operation For 2 Signed Distance.
	 */
	class OPSubstraction : public Graph::Node {
	public:
		OPSubstraction();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> distanceA{};
		Spatial::Spatial<float> distanceB{};

		Spatial::Spatial<float> distanceR{};
		Spatial::Spatial<float> t{};
	};

	/**
	 * Interstection Operation For 2 Signed Distance.
	 */
	class OPIntersection : public Graph::Node {
	public:
		OPIntersection();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> distanceA{};
		Spatial::Spatial<float> distanceB{};

		Spatial::Spatial<float> distanceR{};
		Spatial::Spatial<float> t{};
	};

	/**
	 * Smooth Union Operation For 2 Signed Distance.
	 */
	class OPSmoothUnion : public Graph::Node {
	public:
		OPSmoothUnion();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> distanceA{};
		Spatial::Spatial<float> distanceB{};
		Spatial::Spatial<float> smoothFactor{ 2.0 };

		Spatial::Spatial<float> distanceR{};
		Spatial::Spatial<float> t{};
	};

	/**
	 * Smooth Substraction Operation For 2 Signed Distance.
	 */
	class OPSmoothSubstraction : public Graph::Node {
	public:
		OPSmoothSubstraction();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> distanceA{};
		Spatial::Spatial<float> distanceB{};
		Spatial::Spatial<float> smoothFactor{ 2.0 };

		Spatial::Spatial<float> distanceR{};
		Spatial::Spatial<float> t{};
	};

	/**
	 * Smooth Interstection Operation For 2 Signed Distance.
	 */
	class OPSmoothIntersection : public Graph::Node {
	public:
		OPSmoothIntersection();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> distanceA{};
		Spatial::Spatial<float> distanceB{};
		Spatial::Spatial<float> smoothFactor{ 2.0 };

		Spatial::Spatial<float> distanceR{};
		Spatial::Spatial<float> t{};
	};
}