#include "signed.hpp"

#include <gin/module/registry.hpp>
#include <signed/nodes/3d/primitives.hpp>

void InitializeSignedModule()
{
	Gin::Module::GetRegistry()["Signed/3D/Primitives/Sphere"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::SDSphere>().GetNodeIdx(); };
}

void UninitializeSignedModule()
{

}
