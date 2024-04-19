#pragma once

#include <gin/graph/node.hpp>
#include <gin/field/sampler.hpp>

namespace Gin::Module::Math {

    class HashVector3 : public Graph::Node {
    public:
        HashVector3();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;

        virtual std::string GetName() final;

    private:
        Field::Sampler<Gin::Math::Vector3> in{};

        Field::Sampler<Gin::Math::Scalar> out{};
    };

    class UniformDistribution : public Graph::Node {
    public:
        UniformDistribution();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;

        virtual std::string GetName() final;

    private:
        Field::Sampler<Gin::Math::Scalar> in{};
        Field::Sampler<Gin::Math::Scalar> min{};
        Field::Sampler<Gin::Math::Scalar> max{};

        Field::Sampler<Gin::Math::Scalar> out{};
    };

    class RandomOffset : public Graph::Node {
    public:
        RandomOffset();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;

        virtual std::string GetName() final;

    private:
        Field::Sampler<Gin::Math::Scalar> in{};
        Field::Sampler<Gin::Math::Vector3> min{};
        Field::Sampler<Gin::Math::Vector3> max{};

        Field::Sampler<Gin::Math::Vector3> out{};
    };

/*
	class HashNumber : public Graph::Node {
	public:
		HashNumber();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<double> in{};

		Spatial::Spatial<int> out{};
	};

	class HashVector3 : public Graph::Node {
	public:
		HashVector3();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<Eigen::Vector3<double>> in{};

		Spatial::Spatial<int> out{};
	};

	class UniformDistribution : public Graph::Node {
	public:
		UniformDistribution();

		virtual void Execute(Graph::GraphContext ctx) final;
		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<int> hash{};
		double min{};
		double max{};

		Spatial::Spatial<double> out{};
	};

	class NormalDistribution : public Graph::Node {
	public:
		NormalDistribution();

		virtual void Execute(Graph::GraphContext ctx) final;

		virtual std::string GetName() final;

	private:
		Spatial::Spatial<int> hash{};
		double min{};
		double max{};

		Spatial::Spatial<double> out{};
	};*/

}
