//
// Created by HITO on 17/04/24.
//

#ifndef GIN_SAMPLER_HPP
#define GIN_SAMPLER_HPP

#include <gin/graph/node.hpp>
#include <gin/field/sampler.hpp>
#include <gin/sdf/primitive.hpp>

namespace Gin::Module::Signed {

    class SDSampler : public Graph::Node {
    public:
        SDSampler();

        virtual void Initialize(Graph::GraphContext ctx) final;

        virtual void Execute(Graph::GraphContext ctx) final;

        virtual std::string GetName() final;

    private:
        std::shared_ptr<SDF::Primitive> primitive{};
        Field::Sampler<Math::Vector3> position{};

        Field::Sampler<float> distance{};
    };

}

#endif //GIN_SAMPLER_HPP
