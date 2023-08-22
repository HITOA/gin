#pragma once

#define FASTNOISE_STATIC_LIB

#include <gin/graph/node.hpp>
#include <FastNoise/FastNoise.h>

namespace Gin::Module::Noise {

	//Noise

	class Simplex : public Graph::Node {
	public:
		Simplex();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		FastNoise::SmartNode<> simplex{};
	};

	class OpenSimplex2 : public Graph::Node {
	public:
		OpenSimplex2();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		FastNoise::SmartNode<> simplex{};
	};

	class OpenSimplex2S : public Graph::Node {
	public:
		OpenSimplex2S();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		FastNoise::SmartNode<> simplex{};
	};

	// Cellular

	class Cellular : public Graph::Node {
	public:
		Cellular();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		FastNoise::SmartNode<> cellular{};
	};

	// Fractal

	class FractalFBm : public Graph::Node {
	public:
		FractalFBm();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		FastNoise::SmartNode<> source{};
		float gain{ 0.5 };
		int octaves{ 4 };
		float lacunarity{ 2.5 };

		FastNoise::SmartNode<> fractal{};
	};

	class FractalRidged : public Graph::Node {
	public:
		FractalRidged();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		FastNoise::SmartNode<> source{};
		float gain{ 0.5 };
		int octaves{ 4 };
		float lacunarity{ 2.5 };

		FastNoise::SmartNode<> fractal{};
	};

	//Domain Warp


	class DomainWarp : public Graph::Node {
	public:
		DomainWarp();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		FastNoise::SmartNode<> source{};
		float amplitude{ 1.0 };
		float frequency{ 0.5 };

		FastNoise::SmartNode<> output{};
	};
}
