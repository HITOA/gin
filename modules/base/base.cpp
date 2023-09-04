#include "base.hpp"

#include <gin/module/registry.hpp>

#include <base/nodes/position.hpp>
#include <base/nodes/utils.hpp>

void InitializeBaseModule()
{
	Gin::Module::AddNodesToRegistry<Gin::Module::Base::Position>("Base/Position");

	Gin::Module::AddNodesToRegistry<Gin::Module::Base::Combine<double>>("Base/Utils/Combine");
	Gin::Module::AddNodesToRegistry<Gin::Module::Base::Split3<double>>("Base/Utils/Split3");
	Gin::Module::AddNodesToRegistry<Gin::Module::Base::CombineSpatial<double>>("Base/Utils/CombineSpatial");
	Gin::Module::AddNodesToRegistry<Gin::Module::Base::Split3Spatial<double>>("Base/Utils/Split3Spatial");
}

void UninitializeBaseModule()
{
}
