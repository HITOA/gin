#pragma once

#include <Eigen/Core>

namespace Gin::Math {
	template<typename T, size_t Dim>
	struct Bounds {
		Eigen::Vector<T, Dim> origin{};
		Eigen::Vector<T, Dim> extent{};

		Bounds() = default;
		Bounds(const Eigen::Vector<T, Dim>& origin, const Eigen::Vector<T, Dim> extent) : origin{ origin }, extent{ extent } {};
	};
}
