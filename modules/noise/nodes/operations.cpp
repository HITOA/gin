//
// Created by HITO on 04/05/24.
//

#include "operations.hpp"

Gin::Module::Noise::FilterPointWithNoise::FilterPointWithNoise() {
    AddInputPort("Points", in);
    AddInputPort("Source", source);
    AddInputPort("Frequency", frequency);
    AddInputPort("Seed", seed);
    AddInputPort("Threshold", threshold);

    AddOutputPort("Out", out);
}

void Gin::Module::Noise::FilterPointWithNoise::Initialize(Graph::GraphContext ctx) {
    out = std::make_shared<std::vector<Math::Vector3>>();
}

void Gin::Module::Noise::FilterPointWithNoise::Execute(Graph::GraphContext ctx) {
    if (source.get() == nullptr)
        return;

    for (int i = 0; i < in->size(); ++i) {
        Math::Vector3 point = (*in)[i];
        point *= frequency;
        float nv = source->GenSingle3D(point.x, point.y, point.z, seed);
        if (nv > threshold)
            out->push_back((*in)[i]);
    }
}

std::string Gin::Module::Noise::FilterPointWithNoise::GetName() {
    return "FilterPointWithNoise";
}