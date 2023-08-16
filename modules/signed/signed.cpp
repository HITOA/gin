#include "signed.hpp"

#include <gin/module/registry.hpp>
#include <signed/nodes/3d/primitives.hpp>
#include <signed/nodes/3d/operations.hpp>

void InitializeSignedModule()
{
	//Primitives
	Gin::Module::GetRegistry()["Signed/3D/Primitives/Sphere"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::SDSphere>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Primitives/Box"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::SDBox>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Primitives/Ground"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::SDGround>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Primitives/Plane"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::SDPlane>().GetNodeIdx(); };

	//Operations
	Gin::Module::GetRegistry()["Signed/3D/Operations/Union"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::OPUnion>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Operations/Substraction"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::OPSubstraction>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Operations/Intersection"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::OPIntersection>().GetNodeIdx(); };

	Gin::Module::GetRegistry()["Signed/3D/Operations/SmoothUnion"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::OPSmoothUnion>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Operations/SmoothSubstraction"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::OPSmoothSubstraction>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Operations/SmoothIntersection"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Module::Signed::OPSmoothIntersection>().GetNodeIdx(); };
}

void UninitializeSignedModule()
{

}