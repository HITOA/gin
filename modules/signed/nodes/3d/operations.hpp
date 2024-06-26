#pragma once

#include <gin/graph/node.hpp>
#include <gin/field/sampler.hpp>

namespace Gin::Module::Signed {

	/**
	 * Union Operation For 2 Signed Distance.
	 */
	class OPUnion : public Graph::Node {
	public:
		OPUnion();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
        Field::Sampler<float> distanceA{};
        Field::Sampler<float> distanceB{};

        Field::Sampler<float> distanceR{};
        Field::Sampler<float> t{};
	};

	/**
	 * Substraction Operation For 2 Signed Distance.
	 */
	class OPSubstraction : public Graph::Node {
	public:
		OPSubstraction();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
        Field::Sampler<float> distanceA{};
        Field::Sampler<float> distanceB{};

        Field::Sampler<float> distanceR{};
        Field::Sampler<float> t{};
	};

	/**
	 * Interstection Operation For 2 Signed Distance.
	 */
	class OPIntersection : public Graph::Node {
	public:
		OPIntersection();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
        Field::Sampler<float> distanceA{};
        Field::Sampler<float> distanceB{};

        Field::Sampler<float> distanceR{};
        Field::Sampler<float> t{};
	};

	/**
	 * Smooth Union Operation For 2 Signed Distance.
	 */
	class OPSmoothUnion : public Graph::Node {
	public:
		OPSmoothUnion();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
        Field::Sampler<float> distanceA{};
        Field::Sampler<float> distanceB{};
        Field::Sampler<float> smoothFactor{ 2.0f };

        Field::Sampler<float> distanceR{};
        Field::Sampler<float> t{};
	};

	/**
	 * Smooth Substraction Operation For 2 Signed Distance.
	 */
	class OPSmoothSubstraction : public Graph::Node {
	public:
		OPSmoothSubstraction();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
        Field::Sampler<float> distanceA{};
        Field::Sampler<float> distanceB{};
        Field::Sampler<float> smoothFactor{ 2.0f };

        Field::Sampler<float> distanceR{};
        Field::Sampler<float> t{};
	};

	/**
	 * Smooth Interstection Operation For 2 Signed Distance.
	 */
	/*class OPSmoothIntersection : public Graph::Node {
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
	};*/
}