#pragma once

#include <gin/graph/node.hpp>

namespace Gin::Signed {

	class SDSphere : public Graph::Node {
	public:
		SDSphere();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> radius{ 1.0 };
		Spatial::Spatial<Eigen::Vector3<double>> position{};

		Spatial::Spatial<float> distance{};
	};

}