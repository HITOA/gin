#include "sampler.hpp"

Gin::Module::Noise::Sampler3D::Sampler3D()
{
	AddInputPort("Source", source);
	AddInputPort("Frequency", frequency);
	AddInputPort("Seed", seed);

	AddOutputPort("Output", output);
}

void Gin::Module::Noise::Sampler3D::Execute(Graph::GraphContext ctx)
{
	if (source.get() == nullptr)
		return;

	Eigen::Vector3<double> t = (ctx.bounds.origin - ctx.bounds.extent) / ctx.scale;
	Eigen::Vector3<int> o = Math::Ceil<double, int, 3>(t);
	source->GenUniformGrid3D(output.Data(), o.x(), o.y(), o.z(), output.GetWidth(), output.GetHeight(), output.GetDepth(), ctx.scale * frequency, seed);
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

void Gin::Module::Noise::Sampler2D::Execute(Graph::GraphContext ctx)
{
	if (source.get() == nullptr)
		return;

	Eigen::Vector3<double> t = (ctx.bounds.origin - ctx.bounds.extent) / ctx.scale;
	Eigen::Vector3<int> o = Math::Ceil<double, int, 3>(t);

	auto& rd = FastNoise::New<FastNoise::RemoveDimension>();

	rd->SetSource(source);
	rd->SetRemoveDimension(FastNoise::Dim::Y);

	rd->GenUniformGrid3D(output.Data(), o.x(), o.y(), o.z(), output.GetWidth(), output.GetHeight(), output.GetDepth(), ctx.scale * frequency, seed);
}

std::string Gin::Module::Noise::Sampler2D::GetName()
{
	return "Sampler2D";
}
