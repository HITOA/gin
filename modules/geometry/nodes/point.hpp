//
// Created by HITO on 19/04/24.
//

#ifndef GIN_POINT_HPP
#define GIN_POINT_HPP

#include <gin/graph/node.hpp>
#include <gin/sdf/primitive.hpp>
#include <vector>

namespace Gin::Module::Geometry {

    class PointsCloud : public Gin::Graph::Node {
    public:
        PointsCloud();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;
        virtual std::string GetName() final;

    private:
        float distance{ 1.0 };
        float randomness{ 0.0 };

        std::shared_ptr<std::vector<Math::Vector3>> points{};
    };

    class ScatterSampler : public Gin::Graph::Node {
    public:
        ScatterSampler();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;
        virtual std::string GetName() final;

    private:
        std::shared_ptr<Gin::SDF::Primitive> primitive{};
        std::shared_ptr<std::vector<Math::Vector3>> points{};
        float boudingSphereRadius{ 1.0f };

        Field::Sampler<float> distance{};
    };

}

#endif //GIN_POINT_HPP
