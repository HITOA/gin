#include "sampler.hpp"

Gin::Module::Noise::Sampler3D::Sampler3D()
{
	AddInputPort("Source", source);
	AddInputPort("Frequency", frequency);
	AddInputPort("Seed", seed);

	AddOutputPort("Output", output);
}

void Gin::Module::Noise::Sampler3D::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ ctx.bounds.extent * 2.0 / ctx.scale };
    size.x = std::ceil(size.x);
    size.y = std::ceil(size.y);
    size.z = std::ceil(size.z);

    output.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Noise::Sampler3D::Execute(Graph::GraphContext ctx)
{
	if (source.get() == nullptr)
		return;

    std::shared_ptr<Field::ScalarField<float>> f = output.GetField<Field::ScalarField<float>>();

    Math::Vector3 size{ ctx.bounds.extent * 2.0 / ctx.scale };
    size.x = std::ceil(size.x);
    size.y = std::ceil(size.y);
    size.z = std::ceil(size.z);
	source->GenUniformGrid3D(&(*f)[0], 0, 0, 0, size.x, size.y, size.z, ctx.scale * frequency, seed);
}

std::string Gin::Module::Noise::Sampler3D::GetName()
{
	return "Sampler3D";
}

/*
Gin::Module::Noise::Sampler2D::Sampler2D()
{
	AddInputPort("Source", source);
	AddInputPort("Frequency", frequency);
	AddInputPort("Seed", seed);

	AddOutputPort("Output", output);
}

void Gin::Module::Noise::Sampler2D::Execute(Graph::GraphContext ctx)
{
	if (source.get() == nullptr)
		return;

    Math::Vector3 size{ ctx.bounds.extent * 2.0 / ctx.scale };
    size.x = std::ceil(size.x);
    size.y = std::ceil(size.y);
    size.z = std::ceil(size.z);

	auto rd = FastNoise::New<FastNoise::RemoveDimension>();

	rd->SetSource(source);
	rd->SetRemoveDimension(FastNoise::Dim::Y);

	rd->GenUniformGrid3D(output.Data(), size.x, size.y, size.z, output.GetWidth(), output.GetHeight(), output.GetDepth(), ctx.scale * frequency, seed);
}

std::string Gin::Module::Noise::Sampler2D::GetName()
{
	return "Sampler2D";
}*/
