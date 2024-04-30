#include "math.hpp"

#include <gin/module/registry.hpp>
#include <gin/graph/graph.hpp>

#include <math/nodes/operations.hpp>
#include <math/nodes/transform.hpp>
#include <math/nodes/hash.hpp>
#include <math/nodes/functions.hpp>

void InitializeMathModule()
{
    Gin::Module::AddNodesToRegistry<Gin::Module::Math::Multiply>("Math/Operation/Multiply");
    Gin::Module::AddNodesToRegistry<Gin::Module::Math::Add>("Math/Operation/Add");
    Gin::Module::AddNodesToRegistry<Gin::Module::Math::Substract>("Math/Operation/Substract");
    Gin::Module::AddNodesToRegistry<Gin::Module::Math::Divide>("Math/Operation/Divide");
    Gin::Module::AddNodesToRegistry<Gin::Module::Math::Pow>("Math/Operation/Pow");

    Gin::Module::AddNodesToRegistry<Gin::Module::Math::DomainRepeat>("Math/Transform/Domain Repeat");
    Gin::Module::AddNodesToRegistry<Gin::Module::Math::Translate>("Math/Transform/Translate");
    Gin::Module::AddNodesToRegistry<Gin::Module::Math::Rotate>("Math/Transform/Rotate");
    Gin::Module::AddNodesToRegistry<Gin::Module::Math::Scale>("Math/Transform/Scale");

    Gin::Module::AddNodesToRegistry<Gin::Module::Math::HashVector3>("Math/Hash/Hash Vector3");
    Gin::Module::AddNodesToRegistry<Gin::Module::Math::UniformDistribution>("Math/Hash/Uniform Distribution");

    Gin::Module::AddNodesToRegistry<Gin::Module::Math::Smoothstep>("Math/Function/SmoothStep");

	/*

	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Lerp>("Math/Function/Lerp");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::InvLerp>("Math/Function/InvLerp");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::SmoothStep>("Math/Function/SmoothStep");
	Gin::Module::AddNodesToRegistry<Gin::Module::Math::Remap>("Math/Function/Remap");*/
}

void UninitializeMathModule()
{
}
