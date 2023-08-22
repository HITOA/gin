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
	Eigen::Vector3<double> t = (ctx.bounds.origin - ctx.bounds.extent) / ctx.scale;
	Eigen::Vector3<int> o = Math::Ceil<double, int, 3>(t);
	source->GenUniformGrid3D(output.Data(), o.x(), o.y(), o.z(), output.GetWidth(), output.GetHeight(), output.GetDepth(), ctx.scale * frequency, seed);
}

std::string Gin::Module::Noise::Sampler3D::GetName()
{
	return "Sampler3D";
}
