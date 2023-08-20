#include "math.hpp"

#include <gin/module/registry.hpp>
#include <gin/graph/graph.hpp>

#include <math/nodes/operations.hpp>

void InitializeMathModule()
{
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Multiply<double>>("Math/Multiply");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Add<double>>("Math/Add");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Substract<double>>("Math/Substract");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Divide<double>>("Math/Divide");

	Gin::Module::AddNodesToRegistry<Gin::Module::Math::MultiplySpatial<double>>("Math/MultiplySpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::AddSpatial<double>>("Math/AddSpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::SubstractSpatial<double>>("Math/SubstractSpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::DivideSpatial<double>>("Math/DivideSpatial");
}

void UninitializeMathModule()
{
}
