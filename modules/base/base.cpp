#include "base.hpp"

#include <gin/module/registry.hpp>

#include <base/nodes/position.hpp>
#include <base/nodes/utils.hpp>

void InitializeBaseModule()
{
	Gin::Module::GetRegistry()["Base/Position"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Base::Position>().GetNodeIdx(); };

	Gin::Module::GetRegistry()["Base/Utils/Combine"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Base::Combine<double>>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Base/Utils/CombineSpatial"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Base::CombineSpatial<double>>().GetNodeIdx(); };
}

void UninitializeBaseModule()
{
}
