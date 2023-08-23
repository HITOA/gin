#pragma once

#include <gin/graph/node.hpp>

namespace Gin::Module::Math {

	class Translate : public Graph::Node {
	public:
		Translate();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<Eigen::Vector3<double>> in{};
		Spatial::Spatial<Eigen::Vector3<double>> translate{};

		Spatial::Spatial<Eigen::Vector3<double>> out{};
	};

	class Rotate : public Graph::Node {
	public:
		Rotate();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<Eigen::Vector3<double>> in{};
		Spatial::Spatial<Eigen::Vector3<double>> rotate{};

		Spatial::Spatial<Eigen::Vector3<double>> out{};
	};

	class Scale : public Graph::Node {
	public:
		Scale();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<Eigen::Vector3<double>> in{};
		Spatial::Spatial<Eigen::Vector3<double>> scale{ Eigen::Vector3<double>{ 1.0, 1.0, 1.0 } };

		Spatial::Spatial<Eigen::Vector3<double>> out{};
	};

	class Transform : public Graph::Node {
	public:
		Transform();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<Eigen::Vector3<double>> in{};
		Spatial::Spatial<Eigen::Vector3<double>> translate{};
		Spatial::Spatial<Eigen::Vector3<double>> rotate{};
		Spatial::Spatial<Eigen::Vector3<double>> scale{ Eigen::Vector3<double>{ 1.0, 1.0, 1.0 } };

		Spatial::Spatial<Eigen::Vector3<double>> out{};
	};

	class DomainRepeat : public Graph::Node {
	public:
		DomainRepeat();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<Eigen::Vector3<double>> in{};
		Eigen::Vector3<double> domain{};

		Spatial::Spatial<Eigen::Vector3<double>> out{};
		Spatial::Spatial<Eigen::Vector3<double>> domainPosition{};
	};

}