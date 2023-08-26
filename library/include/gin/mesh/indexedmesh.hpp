#pragma once

#include <gin/mesh/mesh.hpp>

namespace Gin::Mesh {

	struct IndexedMeshVertexData {
		Eigen::Vector3<float> position{};
		Eigen::Vector3<float> normals{};
	};

	class IndexedMesh : public Mesh {
	public:
		virtual void SetVertices(Eigen::Vector3<float>* vertices, size_t count) final;
		virtual void SetNormals(Eigen::Vector3<float>* normals, size_t count) final;
		virtual void SetIndices(int* indices, size_t count) final;
		virtual void Clear() final;

		void BuildIndex();

		std::vector<Eigen::Vector3<float>>& GetVertices();
		std::vector<Eigen::Vector3<float>>& GetNormals();
		std::vector<IndexedMeshVertexData> GetIndexedMeshVertexData();
		std::vector<int>& GetIndices();

	private:
		std::vector<Eigen::Vector3<float>> vertices{};
		std::vector<Eigen::Vector3<float>> normals{};
		std::vector<int> indices{};
	};

}
