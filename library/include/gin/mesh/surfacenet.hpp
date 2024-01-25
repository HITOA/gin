#pragma once

#include <gin/math/type.hpp>
#include <gin/mesh/meshbuilder.hpp>

#define NULL_VERTEX_IDX -1

namespace Gin::Mesh {

	struct SurfaceNetMeshingData {
		std::vector<Math::Vector3> positions{};
		std::vector<Math::Vector3> normals{};
		std::vector<Math::Vector4> colors{};
		std::vector<unsigned int> indices{};

		std::vector<int> posToVertexIdx{};
		std::vector<Math::Vector3Int> vertexIdxToPos{};
	};

	class SurfaceNetMeshBuilder : public MeshBuilder {
	public:
		void Build(MeshBuildData& data) final;
	};

}
