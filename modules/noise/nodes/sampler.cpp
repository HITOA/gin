#include "sampler.hpp"

Gin::Module::Noise::Sampler3D::Sampler3D()
{
	AddInputPort("Source", source);
	AddInputPort("Frequency", frequency);
	AddInputPort("Seed", seed);

	AddOutputPort("Output", output);
}

void Gin::Module::Noise::Sampler3D::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    output.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Noise::Sampler3D::Execute(Graph::GraphContext ctx)
{
	if (source.get() == nullptr)
		return;

    std::shared_ptr<Field::ScalarField<float>> f = output.GetField<Field::ScalarField<float>>();

	source->GenUniformGrid3D(&(*f)[0], 0, 0, 0, f->GetVecWidth(), f->GetHeight(), f->GetDepth(), ctx.scale * frequency, seed);
}

std::string Gin::Module::Noise::Sampler3D::GetName()
{
	return "Sampler3D";
}


Gin::Module::Noise::Sampler2D::Sampler2D()
{
	AddInputPort("Source", source);
	AddInputPort("Frequency", frequency);
	AddInputPort("Seed", seed);

	AddOutputPort("Output", output);
}

void Gin::Module::Noise::Sampler2D::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    output.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Noise::Sampler2D::Execute(Graph::GraphContext ctx)
{
	if (source.get() == nullptr)
		return;


    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

	auto rd = FastNoise::New<FastNoise::RemoveDimension>();

	rd->SetSource(source);
	rd->SetRemoveDimension(FastNoise::Dim::Y);

    std::shared_ptr<Field::ScalarField<float>> f = output.GetField<Field::ScalarField<float>>();

	rd->GenUniformGrid3D(&(*f)[0], 0, 0, 0, f->GetVecWidth(), f->GetHeight(), f->GetDepth(), ctx.scale * frequency, seed);
}

std::string Gin::Module::Noise::Sampler2D::GetName()
{
	return "Sampler2D";
}
