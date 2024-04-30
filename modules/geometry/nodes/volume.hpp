//
// Created by HITO on 29/04/24.
//

#ifndef GIN_VOLUME_HPP
#define GIN_VOLUME_HPP

#include <gin/graph/node.hpp>
#include <gin/field/sampler.hpp>
#include <vector>

namespace Gin::Module::Geometry {

    class CalculateNormals : public Gin::Graph::Node {
    public:
        CalculateNormals();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;
        virtual std::string GetName() final;

    private:
        Field::Sampler<float> distance{};

        Field::Sampler<Math::Vector3> normal{};
    };

}

#endif //GIN_VOLUME_HPP
