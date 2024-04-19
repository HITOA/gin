//
// Created by HITO on 17/04/24.
//

#include "sampler.hpp"

Gin::Module::Signed::SDSampler::SDSampler()
{
    AddInputPort("In", primitive);
    AddInputPort("Position", position);

    AddOutputPort("Distance", distance);
}

void Gin::Module::Signed::SDSampler::Initialize(Graph::GraphContext ctx) {
}

void Gin::Module::Signed::SDSampler::Execute(Graph::GraphContext ctx) {
    distance = primitive->Compute(position);
}

std::string Gin::Module::Signed::SDSampler::GetName() {
    return "SDSampler";
}