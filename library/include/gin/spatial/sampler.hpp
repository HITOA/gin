#pragma once

#include <Eigen/Core>
#include <gin/spatial/spatial.hpp>

namespace Gin::Spatial {

	template<typename T>
	class Sampler {
	public:
		Sampler(Spatial<T> spatial) : spatial{ spatial } {};

		void SetScale(float scale) {
			this->scale = scale;
		}

		void SetBounds(Math::Bounds<double, 3> bounds) {
			this->bounds = bounds;
		}

		inline T nearest(Eigen::Vector3<double> position) {
			Eigen::Vector3<double> mpos = (position - bounds.origin + bounds.extent) / scale;
			size_t x = mpos.x() >= spatial.GetWidth() ? spatial.GetWidth() - 1 : mpos.x() < 0 ? 0 : mpos.x();
			size_t y = mpos.y() >= spatial.GetHeight() ? spatial.GetHeight() - 1 : mpos.y() < 0 ? 0 : mpos.y();
			size_t z = mpos.z() >= spatial.GetDepth() ? spatial.GetDepth() - 1 : mpos.z() < 0 ? 0 : mpos.z();
			size_t idx = x + y * spatial.GetWidth() + z * spatial.GetWidth() * spatial.GetHeight();
			return spatial[idx];
		}

	private:
		float scale{ 1.0f };
		Math::Bounds<double, 3> bounds{};

		Spatial<T> spatial{};
	};

}
