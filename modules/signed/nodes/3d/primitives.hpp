#pragma once

#include <gin/graph/node.hpp>
#include <gin/field/sampler.hpp>

namespace Gin::Module::Signed {

	/**
	 * Signed Distance Function For A Sphere.
	 */
	class SDSphere : public Graph::Node {
	public:
		SDSphere();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
        Field::Sampler<Math::Scalar> radius{ 1.0f };
        Field::Sampler<Math::Vector3> position{};

        Field::Sampler<Math::Scalar> distance{};
	};


	/**
	 * Signed Distance Function For A Sphere.
	 */
	class SDBox : public Graph::Node {
	public:
		SDBox();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
        Field::Sampler<Math::Vector3> bsize{ Math::Vector3{ 1.0f, 1.0f, 1.0f } };
        Field::Sampler<Math::Vector3> position{};

        Field::Sampler<float> distance{};
	};

	/**
	 * Signed Distance Function For A Plane Pointing Up.
	 */
	/*class SDGround : public Graph::Node {
	public:
		SDGround();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> height{ 0.0f };
		Spatial::Spatial<Eigen::Vector3<double>> position{};

		Spatial::Spatial<float> distance{};

	};*/

	/**
	 * Signed Distance Function For A Plane In Any Direction.
	 */
	/*class SDPlane : public Graph::Node {
	public:
		SDPlane();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> height{ 0.0f };
		Spatial::Spatial<Eigen::Vector3<double>> normal{};
		Spatial::Spatial<Eigen::Vector3<double>> position{};

		Spatial::Spatial<float> distance{};
	};*/

	/**
	 * Signed Distance Function For A Torus.
	 */
	/*class SDTorus : public Graph::Node {
	public:
		SDTorus();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> innerRadius{ 1.0f };
		Spatial::Spatial<float> outterRadius{ 1.0f };
		Spatial::Spatial<Eigen::Vector3<double>> position{};

		Spatial::Spatial<float> distance{};
	};*/
}