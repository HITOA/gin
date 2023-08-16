#include "math.hpp"

#include <gin/module/registry.hpp>
#include <gin/graph/graph.hpp>

#include <math/nodes/operations.hpp>

void InitializeMathModule()
{
	Gin::Module::GetRegistry()["Math/Multiply"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Math::Multiply<double>>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Math/Add"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Math::Add<double>>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Math/Substract"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Math::Substract<double>>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Math/Divide"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Math::Divide<double>>().GetNodeIdx(); };

	Gin::Module::GetRegistry()["Math/MultiplySpatial"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Math::MultiplySpatial<double>>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Math/AddSpatial"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Math::AddSpatial<double>>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Math/SubstractSpatial"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Math::SubstractSpatial<double>>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Math/DivideSpatial"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Math::DivideSpatial<double>>().GetNodeIdx(); };
}

void UninitializeMathModule()
{
}
