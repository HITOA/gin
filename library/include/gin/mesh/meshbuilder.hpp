#pragma once

#include <gin/mesh/mesh.hpp>
#include <gin/spatial/sampler.hpp>

namespace Gin::Mesh {

	enum class TriangleWindingOrder {
		CLOCK_WISE,
		COUNTER_CLOCK_WISE
	};

	class MeshBuilder {
	public:
		virtual void Build(Mesh& mesh, Spatial::Sampler<float>& volume, Spatial::Sampler<Eigen::Vector4<float>>& colors) = 0;
		
		inline void SetTriangleWindingOrder(TriangleWindingOrder windingOrder) {
			triangleWindingOrder = windingOrder;
		};

		inline TriangleWindingOrder GetTriangleWindingOrder() {
			return triangleWindingOrder;
		}

	private:
		TriangleWindingOrder triangleWindingOrder{ TriangleWindingOrder::COUNTER_CLOCK_WISE };
	};

}
