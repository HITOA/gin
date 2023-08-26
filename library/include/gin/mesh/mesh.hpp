#pragma once

#include <Eigen/Core>
#include <vector>
#include <unordered_map>
#include <functional>

namespace Gin::Mesh {
	class Mesh {
	public:
		virtual void SetVertices(Eigen::Vector3<float>* vertices, size_t count) = 0;
		virtual void SetNormals(Eigen::Vector3<float>* normals, size_t count) = 0;
		virtual void SetIndices(int* indices, size_t count) = 0;
		virtual void Clear() = 0;
	};
}
