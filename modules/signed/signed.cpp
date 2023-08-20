#include "signed.hpp"

#include <gin/module/registry.hpp>
#include <signed/nodes/3d/primitives.hpp>
#include <signed/nodes/3d/operations.hpp>

void InitializeSignedModule()
{

	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::SDSphere>("Signed/3D/Primitives/Sphere");
	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::SDBox>("Signed/3D/Primitives/Box");
	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::SDGround>("Signed/3D/Primitives/Ground");
	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::SDPlane>("Signed/3D/Primitives/Plane");

	//Operations
	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::OPUnion>("Signed/3D/Operations/Union");
	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::OPSubstraction>("Signed/3D/Operations/Substraction");
	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::OPIntersection>("Signed/3D/Operations/Intersection");

	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::OPSmoothUnion>("Signed/3D/Operations/SmoothUnion");
	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::OPSmoothSubstraction>("Signed/3D/Operations/SmoothSubstraction");
	Gin::Module::AddNodesToRegistry<Gin::Module::Signed::OPSmoothIntersection>("Signed/3D/Operations/SmoothIntersection");
}

void UninitializeSignedModule()
{

}