//
// Created by HITO on 18/04/24.
//

#include <geometry/geometry.hpp>
#include <gin/module/registry.hpp>
#include <geometry/nodes/point.hpp>

void InitializeGeometryModule()
{
    Gin::Module::AddNodesToRegistry<Gin::Module::Geometry::PointsCloud>("Geometry/Points Cloud");
    Gin::Module::AddNodesToRegistry<Gin::Module::Geometry::PointsOnPlane>("Geometry/Points On Plane");
    Gin::Module::AddNodesToRegistry<Gin::Module::Geometry::RandomOffset>("Geometry/Random Offset");
    Gin::Module::AddNodesToRegistry<Gin::Module::Geometry::ScatterSampler>("Geometry/Scatter Sampler");
}

void UninitializeGeometryModule()
{
}
