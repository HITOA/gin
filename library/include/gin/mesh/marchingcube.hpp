#pragma once

#include <Eigen/Core>
#include <gin/math/bounds.hpp>
#include <gin/mesh/meshbuilder.hpp>

namespace Gin::Mesh {

	struct CubeData {
		Eigen::Vector3<double> position[8];
		float distance[8];
	};

	class MarchingCubeMeshBuilder : public MeshBuilder {
	public:
		void Build(Mesh& mesh, Spatial::Sampler<float>& sampler) final;

	private:
		inline unsigned char GetCubeIndex(CubeData& data) {
			return ((data.distance[0] <= 0) & 0x01) |
				(((data.distance[1] <= 0) << 1) & 0x02) |
				(((data.distance[2] <= 0) << 2) & 0x04) |
				(((data.distance[3] <= 0) << 3) & 0x08) |
				(((data.distance[4] <= 0) << 4) & 0x10) |
				(((data.distance[5] <= 0) << 5) & 0x20) |
				(((data.distance[6] <= 0) << 6) & 0x40) |
				(((data.distance[7] <= 0) << 7) & 0x80);
		}

		inline Eigen::Vector2<int> GetVoxelsFromEdge(char edge) {
			int v1 = edge & 7;
			int v2 = edge < 8 ? ((edge + 1) & 3) | (edge & 4) : v1 + 4;
			return Eigen::Vector2<int>{ v1, v2 };
		}

		inline Eigen::Vector3<float> CalculateNormal(Eigen::Vector3<double> point, Spatial::Sampler<float>& sampler) {
			Eigen::Vector3<double> offsets[3]{ { sampler.GetScale(), 0.0f, 0.0f}, {0.0f, sampler.GetScale(), 0.0f}, {0.0f, 0.0f, sampler.GetScale()}};

			Eigen::Vector3<float> d{ sampler.nearest(point), sampler.nearest(point), sampler.nearest(point) };
			Eigen::Vector3<float> r{ sampler.nearest(point + offsets[0]), sampler.nearest(point + offsets[1]), sampler.nearest(point + offsets[2]) };
			return (r - d).normalized();
		}
	public:
		Math::Bounds<double, 3> bounds{};
		float scale{ 1.0f };
	};

}
