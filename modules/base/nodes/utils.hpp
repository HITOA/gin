#pragma once

#include <gin/gin.hpp>

namespace Gin::Module::Base {

	template<typename T>
	class Combine : public Graph::Node {
	public:
		Combine() {
			AddInputPort("X", x);
			AddInputPort("Y", y);
			AddInputPort("Z", z);

			AddOutputPort("Vector2", vec2);
			AddOutputPort("Vector3", vec3);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			vec2.x() = x;
			vec2.y() = y;

			vec3.x() = x;
			vec3.y() = y;
			vec3.z() = z;
		};

		virtual std::string GetName() final {
			return "Combine";
		};

	private:
		T x{};
		T y{};
		T z{};

		Eigen::Vector2<T> vec2{};
		Eigen::Vector3<T> vec3{};
	};

	template<typename T>
	class CombineSpatial : public Graph::Node {
	public:
		CombineSpatial() {
			AddInputPort("X", x);
			AddInputPort("Y", y);
			AddInputPort("Z", z);

			AddOutputPort("Vector2", vec2);
			AddOutputPort("Vector3", vec3);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
				vec2[idx].x() = x[idx];
				vec2[idx].y() = y[idx];
				vec3[idx].x() = x[idx];
				vec3[idx].y() = y[idx];
				vec3[idx].z() = z[idx];
			});
		};

		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final {
			SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
				vec2[idx].x() = x[idx];
				vec2[idx].y() = y[idx];
				vec3[idx].x() = x[idx];
				vec3[idx].y() = y[idx];
				vec3[idx].z() = z[idx];
			});
		};

		virtual std::string GetName() final {
			return "Combine";
		};

	private:
		Spatial::Spatial<T> x{};
		Spatial::Spatial<T> y{};
		Spatial::Spatial<T> z{};

		Spatial::Spatial<Eigen::Vector2<T>> vec2{};
		Spatial::Spatial<Eigen::Vector3<T>> vec3{};
	};

}