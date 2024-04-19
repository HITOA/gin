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
        Field::Sampler<float> volume{};
        Field::Sampler<Math::Vector4> color{ Math::Vector4{ 1.0f, 1.0f, 1.0f, 1.0f } };
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
