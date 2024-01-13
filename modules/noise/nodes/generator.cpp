#include "generator.hpp"

Gin::Module::Noise::Simplex::Simplex()
{
	AddOutputPort("Simplex", simplex);
}

void Gin::Module::Noise::Simplex::Execute(Graph::GraphContext ctx)
{
	simplex = FastNoise::New<FastNoise::Simplex>();
}

std::string Gin::Module::Noise::Simplex::GetName()
{
	return "Simplex";
}

Gin::Module::Noise::OpenSimplex2::OpenSimplex2()
{
	AddOutputPort("OpenSimplex2", simplex);
}

void Gin::Module::Noise::OpenSimplex2::Execute(Graph::GraphContext ctx)
{
	simplex = FastNoise::New<FastNoise::OpenSimplex2>();
}

std::string Gin::Module::Noise::OpenSimplex2::GetName()
{
	return "OpenSimplex2";
}
Gin::Module::Noise::OpenSimplex2S::OpenSimplex2S()
{
	AddOutputPort("OpenSimplex2S", simplex);
}

void Gin::Module::Noise::OpenSimplex2S::Execute(Graph::GraphContext ctx)
{
	simplex = FastNoise::New<FastNoise::OpenSimplex2S>();
}

std::string Gin::Module::Noise::OpenSimplex2S::GetName()
{
	return "OpenSimplex2S";
}

Gin::Module::Noise::Cellular::Cellular()
{
	AddOutputPort("Cellular", cellular);
}

void Gin::Module::Noise::Cellular::Execute(Graph::GraphContext ctx)
{
	cellular = FastNoise::New<FastNoise::CellularValue>();
}

std::string Gin::Module::Noise::Cellular::GetName()
{
	return "Cellular";
}

Gin::Module::Noise::CellularDistance::CellularDistance()
{
	AddOutputPort("Cellular", cellular);
}

void Gin::Module::Noise::CellularDistance::Execute(Graph::GraphContext ctx)
{
	cellular = FastNoise::New<FastNoise::CellularDistance>();
}

std::string Gin::Module::Noise::CellularDistance::GetName()
{
	return "Cellular Distance";
}

Gin::Module::Noise::FractalFBm::FractalFBm()
{
	AddInputPort("Source", source);
	AddInputPort("Gain", gain);
	AddInputPort("Octaves", octaves);
	AddInputPort("Lacunarity", lacunarity);

	AddOutputPort("FBm", fractal);
}

void Gin::Module::Noise::FractalFBm::Execute(Graph::GraphContext ctx)
{
	auto fr = FastNoise::New<FastNoise::FractalFBm>();
	
	fr->SetSource(source);
	fr->SetGain(gain);
	fr->SetOctaveCount(octaves);
	fr->SetLacunarity(lacunarity);

	fractal = fr;
}

std::string Gin::Module::Noise::FractalFBm::GetName()
{
	return "FractalFBm";
}

Gin::Module::Noise::FractalRidged::FractalRidged()
{
	AddInputPort("Source", source);
	AddInputPort("Gain", gain);
	AddInputPort("Octaves", octaves);
	AddInputPort("Lacunarity", lacunarity);

	AddOutputPort("FBm", fractal);
}

void Gin::Module::Noise::FractalRidged::Execute(Graph::GraphContext ctx)
{
	auto fr = FastNoise::New<FastNoise::FractalRidged>();

	fr->SetSource(source);
	fr->SetGain(gain);
	fr->SetOctaveCount(octaves);
	fr->SetLacunarity(lacunarity);

	fractal = fr;
}

std::string Gin::Module::Noise::FractalRidged::GetName()
{
	return "Fractal Ridged";
}

Gin::Module::Noise::DomainWarp::DomainWarp()
{
	AddInputPort("Source", source);
	AddInputPort("Amplitude", amplitude);
	AddInputPort("Frequency", frequency);

	AddOutputPort("Output", output);
}

void Gin::Module::Noise::DomainWarp::Execute(Graph::GraphContext ctx)
{
	auto warp = FastNoise::New<FastNoise::DomainWarpGradient>();

	warp->SetSource(source);
	warp->SetWarpAmplitude(amplitude);
	warp->SetWarpFrequency(frequency);

	output = warp;
}

std::string Gin::Module::Noise::DomainWarp::GetName()
{
	return "Domain Warp";
}
