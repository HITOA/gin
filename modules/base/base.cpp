#include "base.hpp"

#include <gin/module/registry.hpp>

#include <base/nodes/position.hpp>

void InitializeBaseModule()
{
	Gin::Module::GetRegistry()["Base/Position"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Base::Position>().GetNodeIdx(); };
}

void UninitializeBaseModule()
{
}
