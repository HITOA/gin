//
// Created by HITO on 02/05/24.
//

#ifndef GIN_NOISES_HPP
#define GIN_NOISES_HPP

#include <gin/graph/node.hpp>
#include <gin/field/sampler.hpp>

namespace Gin::Module::Signed {

    class Noise {
    public:
        virtual Field::Sampler<float> Compute(Field::Sampler<Math::Vector3> position, float frequency, int seed) = 0;
    };

    class SDSphericalNoise: public Graph::Node {
    public:
        SDSphericalNoise();

        virtual void Initialize(Gin::Graph::GraphContext ctx) final;
        virtual void Execute(Gin::Graph::GraphContext ctx) final;
        virtual std::string GetName() final;

    private:
        std::shared_ptr<Noise> out{};

        class SphericalNoise : public Noise {
        public:
            virtual Field::Sampler<float> Compute(Field::Sampler<Math::Vector3> position, float frequency, int seed) final;
        };
    };

    class SDCubicNoise: public Graph::Node {
    public:
        SDCubicNoise();

        virtual void Initialize(Gin::Graph::GraphContext ctx) final;
        virtual void Execute(Gin::Graph::GraphContext ctx) final;
        virtual std::string GetName() final;

    private:
        std::shared_ptr<Noise> out{};

        class CubicNoise : public Noise {
        public:
            virtual Field::Sampler<float> Compute(Field::Sampler<Math::Vector3> position, float frequency, int seed) final;
        };
    };

    class SDFBMNoiseSampler: public Graph::Node {
    public:
        SDFBMNoiseSampler();

        virtual void Initialize(Gin::Graph::GraphContext ctx) final;
        virtual void Execute(Gin::Graph::GraphContext ctx) final;
        virtual void Execute(Gin::Graph::GraphContext ctx, Thread::ThreadPool& pool);
        virtual std::string GetName() final;

    private:
        std::shared_ptr<Noise> in{};
        Field::Sampler<Math::Vector3> position{};
        Field::Sampler<float> distance{};
        float frequency{ 0.5f };
        float seed{ 1234 };
        int octaves{ 4 };
        float gain{ 0.5f };
        float lacunarity{ 2.0f };

        Field::Sampler<float> out{};
    };

}

#endif //GIN_NOISES_HPP
