#pragma once

#include <gin/math/type.hpp>
#include <vector>
#include <unordered_map>
#include <functional>

namespace Gin::Mesh {
	class Mesh {
	public:
		virtual void SetVertices(Math::Vector3* vertices, size_t count) = 0;
		virtual void SetNormals(Math::Vector3* normals, size_t count) = 0;
		virtual void SetColors(Math::Vector4* colors, size_t count) = 0;
		virtual void SetIndices(unsigned int* indices, size_t count) = 0;
		virtual void Clear() = 0;
	};
}
