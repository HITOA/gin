#include "color.hpp"

#include <gin/module/registry.hpp>
#include <color/nodes/colormath.hpp>

void InitializeColorModule()
{
	Gin::Module::AddNodesToRegistry<Gin::Module::Color::Blend>("Color/Blend");
}

void UninitializeColorModule()
{
}
