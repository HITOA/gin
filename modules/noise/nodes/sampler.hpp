#pragma once

#define FASTNOISE_STATIC_LIB

#include <gin/graph/node.hpp>
#include <FastNoise/FastNoise.h>
#include <gin/field/sampler.hpp>

namespace Gin::Module::Noise {

	class Sampler3D : public Graph::Node {
	public:
		Sampler3D();

        virtual void Initialize(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		FastNoise::SmartNode<> source{};
		float frequency{ 0.5 };
		int seed{ 1234 };

        Field::Sampler<float> output{};

	};
    /*
	class Sampler2D : public Graph::Node {
	public:
		Sampler2D();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		FastNoise::SmartNode<> source{};
		float frequency{ 0.5 };
		int seed{ 1234 };

		Spatial::Spatial<float> output{};
	};*/

}

