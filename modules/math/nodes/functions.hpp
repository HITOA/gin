#pragma once

#include <gin/graph/node.hpp>

namespace Gin::Module::Math {

	class Lerp : public Graph::Node {
	public:
		Lerp();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> a{};
		Spatial::Spatial<float> b{};
		Spatial::Spatial<float> t{};

		Spatial::Spatial<float> r{};
	};

	class InvLerp : public Graph::Node {
	public:
		InvLerp();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> a{};
		Spatial::Spatial<float> b{};
		Spatial::Spatial<float> t{};

		Spatial::Spatial<float> r{};
	};

	class SmoothStep : public Graph::Node {
	public:
		SmoothStep();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> a{};
		Spatial::Spatial<float> min{};
		Spatial::Spatial<float> max{};

		Spatial::Spatial<float> r{};
	};

	class Remap : public Graph::Node {
	public:
		Remap();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<float> a{};
		Spatial::Spatial<float> minA{};
		Spatial::Spatial<float> maxA{};
		Spatial::Spatial<float> minB{};
		Spatial::Spatial<float> maxB{};

		Spatial::Spatial<float> r{};
	};
}