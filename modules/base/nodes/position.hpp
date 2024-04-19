#pragma once

#include <gin/gin.hpp>

namespace Gin::Module::Base {

	class Position : public Graph::Node {
	public:
		Position();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;
		virtual std::string GetName() final;

	private:
        Field::Sampler<Math::Vector3> position{};
	};

}