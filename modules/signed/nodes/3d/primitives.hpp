#pragma once

#include <gin/graph/node.hpp>

namespace Gin::Module::Signed {

	/**
	 * Signed Distance Function For A Sphere.
	 */
	class SDSphere : public Graph::Node {
	public:
		SDSphere();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> radius{ 1.0f };
		Spatial::Spatial<Eigen::Vector3<double>> position{};

		Spatial::Spatial<float> distance{};
	};

	/**
	 * Signed Distance Function For A Sphere.
	 */
	class SDBox : public Graph::Node {
	public:
		SDBox();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<Eigen::Vector3<double>> size{ Eigen::Vector3<double>{ 1.0f, 1.0f, 1.0f } };
		Spatial::Spatial<Eigen::Vector3<double>> position{};

		Spatial::Spatial<float> distance{};
	};

	/**
	 * Signed Distance Function For A Plane Pointing Up.
	 */
	class SDGround : public Graph::Node {
	public:
		SDGround();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> height{ 0.0f };
		Spatial::Spatial<Eigen::Vector3<double>> position{};

		Spatial::Spatial<float> distance{};

	};

	/**
	 * Signed Distance Function For A Plane In Any Direction.
	 */
	class SDPlane : public Graph::Node {
	public:
		SDPlane();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> height{ 0.0f };
		Spatial::Spatial<Eigen::Vector3<double>> normal{};
		Spatial::Spatial<Eigen::Vector3<double>> position{};

		Spatial::Spatial<float> distance{};
	};
}