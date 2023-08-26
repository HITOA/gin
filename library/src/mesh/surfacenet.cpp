#include <gin/mesh/surfacenet.hpp>

#include <gin/math/interpolation.hpp>

void Gin::Mesh::SurfaceNetMeshBuilder::Build(Mesh& mesh, Spatial::Sampler<float>& sampler)
{
    for (double z = -bounds.extent.z(); z < bounds.extent.z(); z += scale) {
        for (double y = -bounds.extent.y(); y < bounds.extent.y(); y += scale) {
            for (double x = -bounds.extent.x(); x < bounds.extent.x(); x += scale) {
                
                float v000 = sampler.nearest(Eigen::Vector3<double>{ x, y, z });
                float v100 = sampler.nearest(Eigen::Vector3<double>{ x + scale, y, z });
                float v010 = sampler.nearest(Eigen::Vector3<double>{ x, y + scale, z });
                float v110 = sampler.nearest(Eigen::Vector3<double>{ x + scale, y + scale, z });
                float v001 = sampler.nearest(Eigen::Vector3<double>{ x, y, z + scale });
                float v101 = sampler.nearest(Eigen::Vector3<double>{ x + scale, y, z + scale });
                float v011 = sampler.nearest(Eigen::Vector3<double>{ x, y + scale, z + scale });
                float v111 = sampler.nearest(Eigen::Vector3<double>{ x + scale, y + scale, z + scale });

            }
        }
    }
}
