#include "terrain.hpp"

#include <gin/module/registry.hpp>

#include <terrain/nodes/features.hpp>
#include <terrain/nodes/operations.hpp>

void InitializeTerrainModule()
{
	Gin::Module::AddNodesToRegistry<Gin::Module::Terrain::Overhang>("Terrain/Features/Overhang");

	Gin::Module::AddNodesToRegistry<Gin::Module::Terrain::TerrainMix>("Terrain/Operation/Terrain Mix");
}

void UninitializeTerrainModule()
{
}
