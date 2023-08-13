#include "signed.hpp"

#include <gin/module/registry.hpp>
#include <signed/nodes/primitives.hpp>

void InitializeSignedModule()
{
	Gin::Module::GetRegistry()["Signed/Primitives/Sphere"] = [&](Gin::Graph::Graph& graph) { return graph.AddNode<Gin::Signed::SDSphere>().GetNodeIdx(); };
}

void UninitializeSignedModule()
{

}
