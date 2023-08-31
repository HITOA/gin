#pragma once

#include <gin/graph/node.hpp>

namespace Gin::Module::Math {

	template<typename T>
	class Multiply : public Graph::Node {
	public:
		Multiply() {
			AddInputPort("A", a);
			AddInputPort("B", b);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			r = a * b;
		}

		virtual std::string GetName() final {
			return "Multiply";
		}

	private:
		T a{};
		T b{};

		T r{};
	};

	template<typename T>
	class Add : public Graph::Node {
	public:
		Add() {
			AddInputPort("A", a);
			AddInputPort("B", b);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			r = a + b;
		}

		virtual std::string GetName() final {
			return "Add";
		}

	private:
		T a{};
		T b{};

		T r{};
	};

	template<typename T>
	class Substract : public Graph::Node {
	public:
		Substract() {
			AddInputPort("A", a);
			AddInputPort("B", b);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			r = a - b;
		}

		virtual std::string GetName() final {
			return "Substract";
		}

	private:
		T a{};
		T b{};

		T r{};
	};

	template<typename T>
	class Divide : public Graph::Node {
	public:
		Divide() {
			AddInputPort("A", a);
			AddInputPort("B", b);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			r = a / b;
		}

		virtual std::string GetName() final {
			return "Divide";
		}

	private:
		T a{};
		T b{};

		T r{};
	};

	template<typename T>
	class Pow : public Graph::Node {
	public:
		Pow() {
			AddInputPort("A", a);
			AddInputPort("Power", p);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			r = pow(a, p);
		}

		virtual std::string GetName() final {
			return "Pow";
		}

	private:
		T a{};
		T p{};

		T r{};
	};

	//Spatial

	template<typename T>
	class MultiplySpatial : public Graph::Node {
	public:
		MultiplySpatial() {
			AddInputPort("A", a);
			AddInputPort("B", b);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = a[idx] * b[idx];
			});
		}

		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final {
			SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = a[idx] * b[idx];
				});
		}

		virtual std::string GetName() final {
			return "Multiply";
		}

	private:
		Spatial::Spatial<T> a{};
		Spatial::Spatial<T> b{};

		Spatial::Spatial<T> r{};
	};

	template<typename T>
	class AddSpatial : public Graph::Node {
	public:
		AddSpatial() {
			AddInputPort("A", a);
			AddInputPort("B", b);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = a[idx] + b[idx];
			});
		}

		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final {
			SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = a[idx] + b[idx];
				});
		}

		virtual std::string GetName() final {
			return "Add";
		}

	private:
		Spatial::Spatial<T> a{};
		Spatial::Spatial<T> b{};

		Spatial::Spatial<T> r{};
	};

	template<typename T>
	class SubstractSpatial : public Graph::Node {
	public:
		SubstractSpatial() {
			AddInputPort("A", a);
			AddInputPort("B", b);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = a[idx] - b[idx];
				});
		}

		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final {
			SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = a[idx] - b[idx];
			});
		}

		virtual std::string GetName() final {
			return "Substract";
		}

	private:
		Spatial::Spatial<T> a{};
		Spatial::Spatial<T> b{};

		Spatial::Spatial<T> r{};
	};

	template<typename T>
	class DivideSpatial : public Graph::Node {
	public:
		DivideSpatial() {
			AddInputPort("A", a);
			AddInputPort("B", b);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = a[idx] / b[idx];
				});
		}

		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final {
			SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = a[idx] / b[idx];
				});
		}

		virtual std::string GetName() final {
			return "Divide";
		}

	private:
		Spatial::Spatial<T> a{};
		Spatial::Spatial<T> b{};

		Spatial::Spatial<T> r{};
	};

	template<typename T>
	class PowSpatial : public Graph::Node {
	public:
		PowSpatial() {
			AddInputPort("A", a);
			AddInputPort("Power", p);

			AddOutputPort("Result", r);
		}

		virtual void Execute(Graph::GraphContext ctx) final {
			SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = pow(a[idx], p[idx]);
			});
		}

		virtual void Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool) final {
			SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
				r[idx] = pow(a[idx], p[idx]);
			});
		}

		virtual std::string GetName() final {
			return "Pow";
		}

	private:
		Spatial::Spatial<T> a{};
		Spatial::Spatial<T> p{};

		Spatial::Spatial<T> r{};
	};
}