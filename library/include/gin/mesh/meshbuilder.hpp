#pragma once

#include <gin/mesh/mesh.hpp>
#include <gin/spatial/sampler.hpp>

namespace Gin::Mesh {

	class MeshBuilder {
	public:
		virtual void Build(Mesh& mesh, Spatial::Sampler<float>& field) = 0;
	};

}
