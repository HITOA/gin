#pragma once

#include <gin/gin.hpp>

namespace Gin::Module::Base {

	class Position : public Graph::Node {
	public:
		Position();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<Eigen::Vector3<double>> position{};
	};

}