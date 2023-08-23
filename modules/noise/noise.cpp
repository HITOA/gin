#include "noise.hpp"

#include <gin/module/registry.hpp>

#include <noise/nodes/generator.hpp>
#include <noise/nodes/sampler.hpp>

void InitializeNoiseModule()
{
	Gin::Module::AddNodesToRegistry<Gin::Module::Noise::Simplex>("FastNoise2/Generator/Simplex");
	Gin::Module::AddNodesToRegistry<Gin::Module::Noise::OpenSimplex2>("FastNoise2/Generator/OpenSimplex2");
	Gin::Module::AddNodesToRegistry<Gin::Module::Noise::OpenSimplex2S>("FastNoise2/Generator/OpenSimplex2S");
	Gin::Module::AddNodesToRegistry<Gin::Module::Noise::Cellular>("FastNoise2/Generator/Cellular");

	Gin::Module::AddNodesToRegistry<Gin::Module::Noise::FractalFBm>("FastNoise2/Fractal/FractalFBm");
	Gin::Module::AddNodesToRegistry<Gin::Module::Noise::FractalRidged>("FastNoise2/Fractal/Fractal Ridged");

	Gin::Module::AddNodesToRegistry<Gin::Module::Noise::DomainWarp>("FastNoise2/Warp/Domain Warp");

	Gin::Module::AddNodesToRegistry<Gin::Module::Noise::Sampler3D>("FastNoise2/Sampler3D");
	Gin::Module::AddNodesToRegistry<Gin::Module::Noise::Sampler2D>("FastNoise2/Sampler2D");
}

void UninitializeNoiseModule()
{
}
