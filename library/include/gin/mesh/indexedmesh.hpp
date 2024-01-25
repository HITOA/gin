#pragma once

#include <gin/math/type.hpp>
#include <gin/mesh/mesh.hpp>

namespace Gin::Mesh {

	class IndexedMesh : public Mesh {
	public:
		virtual void SetVertices(Math::Vector3* vertices, size_t count) final;
		virtual void SetNormals(Math::Vector3* normals, size_t count) final;
		virtual void SetColors(Math::Vector4* colors, size_t count) final;
		virtual void SetIndices(unsigned int* indices, size_t count) final;
		virtual void Clear() final;

		void BuildIndex();
		void RecalculateNormals();

		std::vector<Math::Vector3>& GetVertices();
		std::vector<Math::Vector3>& GetNormals();
		std::vector<Math::Vector4>& GetColors();
		std::vector<unsigned int>& GetIndices();

	private:
		std::vector<Math::Vector3> vertices{};
		std::vector<Math::Vector3> normals{};
		std::vector<Math::Vector4> colors{};
		std::vector<unsigned int> indices{};
	};

}
