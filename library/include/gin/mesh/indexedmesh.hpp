#pragma once

#include <gin/mesh/mesh.hpp>

namespace Gin::Mesh {

	class IndexedMesh : public Mesh {
	public:
		virtual void SetVertices(Eigen::Vector3<float>* vertices, size_t count) final;
		virtual void SetNormals(Eigen::Vector3<float>* normals, size_t count) final;
		virtual void SetColors(Eigen::Vector4<float>* colors, size_t count) final;
		virtual void SetIndices(unsigned int* indices, size_t count) final;
		virtual void Clear() final;

		void BuildIndex();
		void RecalculateNormals();

		std::vector<Eigen::Vector3<float>>& GetVertices();
		std::vector<Eigen::Vector3<float>>& GetNormals();
		std::vector<Eigen::Vector4<float>>& GetColors();
		std::vector<unsigned int>& GetIndices();

	private:
		std::vector<Eigen::Vector3<float>> vertices{};
		std::vector<Eigen::Vector3<float>> normals{};
		std::vector<Eigen::Vector4<float>> colors{};
		std::vector<unsigned int> indices{};
	};

}
