#pragma once

#include <Eigen/Core>
#include <gin/math/bounds.hpp>
#include <gin/mesh/meshbuilder.hpp>

#define NULL_VERTEX_IDX -1

namespace Gin::Mesh {

	struct SurfaceNetMeshingData {
		std::vector<Eigen::Vector3<float>> positions{};
		std::vector<Eigen::Vector3<float>> normals{};
		std::vector<Eigen::Vector4<float>> colors{};
		std::vector<int> indices{};

		std::vector<int> posToVertexIdx{};
		std::vector<Eigen::Vector3<int>> vertexIdxToPos{};
	};

	class SurfaceNetMeshBuilder : public MeshBuilder {
	public:
		void Build(Mesh& mesh, Spatial::Sampler<float>& volume, Spatial::Sampler<Eigen::Vector4<float>>& colors) final;
	};

}
