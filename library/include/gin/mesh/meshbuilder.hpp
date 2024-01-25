#pragma once

#include <gin/mesh/mesh.hpp>
#include <gin/field/sampler.hpp>

namespace Gin::Mesh {

	enum class TriangleWindingOrder {
		CLOCK_WISE,
		COUNTER_CLOCK_WISE
	};

    struct MeshBuildData {
        Mesh* mesh{ nullptr };
        Math::Scalar scale{ 1.0f };
        Math::Bounds bounds{};
        Field::ScalarField<Math::Scalar>* volume{ nullptr };
        Field::VectorizedVector4Field* color{ nullptr };
    };

	class MeshBuilder {
	public:
		virtual void Build(MeshBuildData& data) = 0;
		
		inline void SetTriangleWindingOrder(TriangleWindingOrder windingOrder) {
			triangleWindingOrder = windingOrder;
		};

		inline TriangleWindingOrder GetTriangleWindingOrder() {
			return triangleWindingOrder;
		}

    protected:
		TriangleWindingOrder triangleWindingOrder{ TriangleWindingOrder::COUNTER_CLOCK_WISE };
	};

}
