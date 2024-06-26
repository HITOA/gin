#pragma once

#include <gin/graph/node.hpp>
#include <gin/field/sampler.hpp>
#include <gin/sdf/primitive.hpp>

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

        std::shared_ptr<SDF::Primitive> primitive{};

        class SpherePrimitive : public SDF::Primitive {
        public:
            virtual Field::Sampler<float> Compute(Field::Sampler<Math::Vector3> position) final;

        public:
            Field::Sampler<Math::Scalar> radius{ 1.0f };
        };
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

        std::shared_ptr<SDF::Primitive> primitive{};

        class BoxPrimitive : public SDF::Primitive {
        public:
            virtual Field::Sampler<float> Compute(Field::Sampler<Math::Vector3> position) final;

        public:
            Field::Sampler<Math::Vector3> bsize{ Math::Vector3{ 1.0f } };
        };
	};

	/**
	 * Signed Distance Function For A Plane Pointing Up.
	 */
	class SDGround : public Graph::Node {
	public:
		SDGround();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
        Field::Sampler<float> height{ 0.0f };

        std::shared_ptr<SDF::Primitive> primitive{};

        class GroundPrimitive : public SDF::Primitive {
        public:
            virtual Field::Sampler<float> Compute(Field::Sampler<Math::Vector3> position) final;

        public:
            Field::Sampler<float> height{ 0.0f };
        };

	};

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