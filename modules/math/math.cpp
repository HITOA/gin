#include "math.hpp"

#include <gin/module/registry.hpp>
#include <gin/graph/graph.hpp>

#include <math/nodes/operations.hpp>

void InitializeMathModule()
{
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Multiply<float>>("Math/Multiply");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Add<float>>("Math/Add");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Substract<float>>("Math/Substract");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Divide<float>>("Math/Divide");

	Gin::Module::AddNodesToRegistry<Gin::Module::Math::MultiplySpatial<float>>("Math/MultiplySpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::AddSpatial<float>>("Math/AddSpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::SubstractSpatial<float>>("Math/SubstractSpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::DivideSpatial<float>>("Math/DivideSpatial");
}

void UninitializeMathModule()
{
}
