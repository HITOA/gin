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
			AddInputPort("W", w);

			AddOutputPort("Vector2", vec2);
			AddOutputPort("Vector3", vec3);
			AddOutputPort("Color", color);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			vec2.x() = x;
			vec2.y() = y;

			vec3.x() = x;
			vec3.y() = y;
			vec3.z() = z;

			color.x() = (float)x;
			color.y() = (float)y;
			color.z() = (float)z;
			color.w() = (float)w;
		};

		virtual std::string GetName() final {
			return "Combine";
		};

	private:
		T x{};
		T y{};
		T z{};
		T w{};

		Eigen::Vector2<T> vec2{};
		Eigen::Vector3<T> vec3{};
		Eigen::Vector4<float> color{};
	};

	template<typename T>
	class Split3 : public Graph::Node {
	public:
		Split3() {
			AddInputPort("Vector3", vec3);

			AddOutputPort("X", x);
			AddOutputPort("Y", y);
			AddOutputPort("Z", z);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			x = vec3.x();
			y = vec3.y();
			z = vec3.z();
		};

		virtual std::string GetName() final {
			return "Split3";
		};

	private:
		Eigen::Vector3<T> vec3{};

		T x{};
		T y{};
		T z{};
	};

	template<typename T>
	class CombineSpatial : public Graph::Node {
	public:
		CombineSpatial() {
			AddInputPort("X", x);
			AddInputPort("Y", y);
			AddInputPort("Z", z);
			AddInputPort("W", w);

			AddOutputPort("Vector2", vec2);
			AddOutputPort("Vector3", vec3);
			AddOutputPort("Color", color);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
				vec2[idx].x() = x[idx];
				vec2[idx].y() = y[idx];

				vec3[idx].x() = x[idx];
				vec3[idx].y() = y[idx];
				vec3[idx].z() = z[idx];

				color[idx].x() = (float)x[idx];
				color[idx].y() = (float)y[idx];
				color[idx].z() = (float)z[idx];
				color[idx].w() = (float)w[idx];
			});
		};

		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final {
			SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
				vec2[idx].x() = x[idx];
				vec2[idx].y() = y[idx];

				vec3[idx].x() = x[idx];
				vec3[idx].y() = y[idx];
				vec3[idx].z() = z[idx];

				color[idx].x() = (float)x[idx];
				color[idx].y() = (float)y[idx];
				color[idx].z() = (float)z[idx];
				color[idx].w() = (float)w[idx];
			});
		};

		virtual std::string GetName() final {
			return "Combine";
		};

	private:
		Spatial::Spatial<T> x{};
		Spatial::Spatial<T> y{};
		Spatial::Spatial<T> z{};
		Spatial::Spatial<T> w{};

		Spatial::Spatial<Eigen::Vector2<T>> vec2{};
		Spatial::Spatial<Eigen::Vector3<T>> vec3{};
		Spatial::Spatial<Eigen::Vector4<float>> color{};
	};

	template<typename T>
	class Split3Spatial : public Graph::Node {
	public:
		Split3Spatial() {
			AddInputPort("Vector3", vec3);

			AddOutputPort("X", x);
			AddOutputPort("Y", y);
			AddOutputPort("Z", z);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
				x[idx] = vec3[idx].x();
				y[idx] = vec3[idx].y();
				z[idx] = vec3[idx].z();
			});
		};

		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final {
			SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
				x[idx] = vec3[idx].x();
				y[idx] = vec3[idx].y();
				z[idx] = vec3[idx].z();
				});
		};

		virtual std::string GetName() final {
			return "Split3";
		};

	private:
		Spatial::Spatial<Eigen::Vector3<T>> vec3{};

		Spatial::Spatial<T> x{};
		Spatial::Spatial<T> y{};
		Spatial::Spatial<T> z{};
	};

}