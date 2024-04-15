#include "base.hpp"

#include <gin/module/registry.hpp>

#include <base/nodes/position.hpp>
#include <base/nodes/utils.hpp>

void InitializeBaseModule()
{
	Gin::Module::AddNodesToRegistry<Gin::Module::Base::Position>("Base/Position");

	Gin::Module::AddNodesToRegistry<Gin::Module::Base::Combine>("Base/Utils/Combine");
	Gin::Module::AddNodesToRegistry<Gin::Module::Base::Split>("Base/Utils/Split3");
}

void UninitializeBaseModule()
{
}
