#pragma once

#include <Eigen/Core>
#include <vector>
#include <unordered_map>
#include <functional>

namespace Gin::Mesh {

	//Source : https://wjngkoh.wordpress.com/2015/03/04/c-hash-function-for-eigen-matrix-and-vector/
	template<typename T>
	struct matrix_hash {
		std::size_t operator()(T const& matrix) const {
			// Note that it is oblivious to the storage order of Eigen matrix (column- or
			// row-major). It will give you the same hash value for two different matrices if they
			// are the transpose of each other in different storage order.
			size_t seed = 0;
			for (size_t i = 0; i < matrix.size(); ++i) {
				auto elem = *(matrix.data() + i);
				seed ^= std::hash<typename T::Scalar>()(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};

	struct VertexData {
		Eigen::Vector3<float> position{};
		Eigen::Vector3<float> normal{};
	};

	class Mesh {
	public:
		virtual void AddVertexData(VertexData& vertexData) = 0;
		virtual void ClearVertexData() = 0;
	};

	class IndexedMesh : public Mesh {
	public:
		void AddVertexData(VertexData& vertexData) final;
		void ClearVertexData() final;
		size_t GetVertexBufferSize();
		size_t GetIndexBufferSize();
		VertexData* GetVertexBuffer();
		int* GetIndexBuffer();
		void RecalculateNormal();

	private:
		std::unordered_map<Eigen::Vector3<float>, int, matrix_hash<Eigen::Vector3<float>>> vertexIndexTable{};
		std::vector<VertexData> vertices{};
		std::vector<int> indices{};
	};
}
