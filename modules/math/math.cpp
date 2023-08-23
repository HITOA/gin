#include "math.hpp"

#include <gin/module/registry.hpp>
#include <gin/graph/graph.hpp>

#include <math/nodes/operations.hpp>
#include <math/nodes/transform.hpp>

void InitializeMathModule()
{
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Multiply<float>>("Math/Operation/Multiply");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Add<float>>("Math/Operation/Add");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Substract<float>>("Math/Operation/Substract");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Divide<float>>("Math/Operation/Divide");

	Gin::Module::AddNodesToRegistry<Gin::Module::Math::MultiplySpatial<float>>("Math/Operation/MultiplySpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::AddSpatial<float>>("Math/Operation/AddSpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::SubstractSpatial<float>>("Math/Operation/SubstractSpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::DivideSpatial<float>>("Math/Operation/DivideSpatial");

	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Translate>("Math/Transform/Translate");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Rotate>("Math/Transform/Rotate");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Scale>("Math/Transform/Scale");
}

void UninitializeMathModule()
{
}
