//
// Created by HITO on 04/05/24.
//

#ifndef GIN_OPERATIONS_HPP
#define GIN_OPERATIONS_HPP


#include <gin/graph/node.hpp>
#include <gin/field/sampler.hpp>
#include "sampler.hpp"

namespace Gin::Module::Noise {

    class FilterPointWithNoise : public Graph::Node {
    public:
        FilterPointWithNoise();

        virtual void Initialize(Graph::GraphContext ctx) final;
        virtual void Execute(Graph::GraphContext ctx) final;

        virtual std::string GetName() final;

    private:
        std::shared_ptr<std::vector<Math::Vector3>> in{};
        FastNoise::SmartNode<> source{};
        float frequency{ 0.5 };
        int seed{ 1234 };
        float threshold{ 0.0 };

        std::shared_ptr<std::vector<Math::Vector3>> out{};
    };

}



#endif //GIN_OPERATIONS_HPP
