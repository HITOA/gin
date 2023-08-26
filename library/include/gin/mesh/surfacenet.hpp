#pragma once

#include <Eigen/Core>
#include <gin/math/bounds.hpp>
#include <gin/mesh/meshbuilder.hpp>

namespace Gin::Mesh {

	class SurfaceNetMeshBuilder : public MeshBuilder {
	public:
		void Build(Mesh& mesh, Spatial::Sampler<float>& sampler) final;

	public:
		Math::Bounds<double, 3> bounds{};
		float scale{ 1.0f };
	};

}
