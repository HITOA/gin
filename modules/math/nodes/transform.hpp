#pragma once

#include <gin/graph/node.hpp>
#include <gin/math/type.hpp>

namespace Gin::Module::Math {

    class DomainRepeat : public Graph::Node {
    public:
        DomainRepeat();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;

        virtual std::string GetName() final;

    private:
        Field::Sampler<Gin::Math::Vector3> in{};
        Field::Sampler<Gin::Math::Vector3> domainSize{};

        Field::Sampler<Gin::Math::Vector3> out{};
        Field::Sampler<Gin::Math::Vector3> domainPosition{};
    };

    class Translate : public Graph::Node {
    public:
        Translate();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;

        virtual std::string GetName() final;

    private:
        Field::Sampler<Gin::Math::Vector3> in{};
        Field::Sampler<Gin::Math::Vector3> translation{};

        Field::Sampler<Gin::Math::Vector3> out{};
    };

    class Rotate : public Graph::Node {
    public:
        Rotate();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;

        virtual std::string GetName() final;

    private:
        Field::Sampler<Gin::Math::Vector3> in{};
        Field::Sampler<Gin::Math::Vector3> rotation{};

        Field::Sampler<Gin::Math::Vector3> out{};
    };

    class Scale : public Graph::Node {
    public:
        Scale();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;

        virtual std::string GetName() final;

    private:
        Field::Sampler<Gin::Math::Vector3> in{};
        Field::Sampler<Gin::Math::Vector3> scale{};

        Field::Sampler<Gin::Math::Vector3> out{};
    };
}