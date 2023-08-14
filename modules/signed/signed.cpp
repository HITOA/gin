#include "signed.hpp"

#include <gin/module/registry.hpp>
#include <signed/nodes/3d/primitives.hpp>
#include <signed/nodes/3d/operations.hpp>

void InitializeSignedModule()
{
	//Primitives
	Gin::Module::GetRegistry()["Signed/3D/Primitives/Sphere"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::SDSphere>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Primitives/Ground"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::SDGround>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Primitives/Plane"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::SDPlane>().GetNodeIdx(); };

	//Operations
	Gin::Module::GetRegistry()["Signed/3D/Opeartions/Union"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::OPUnion>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Opeartions/Substraction"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::OPSubstraction>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Opeartions/Intersection"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::OPIntersection>().GetNodeIdx(); };

	Gin::Module::GetRegistry()["Signed/3D/Opeartions/SmoothUnion"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::OPSmoothUnion>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Opeartions/SmoothSubstraction"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::OPSmoothSubstraction>().GetNodeIdx(); };
	Gin::Module::GetRegistry()["Signed/3D/Opeartions/SmoothIntersection"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::OPSmoothIntersection>().GetNodeIdx(); };
}

void UninitializeSignedModule()
{

}
