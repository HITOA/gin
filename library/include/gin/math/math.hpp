#pragma once

#include <Eigen/Core>

namespace Gin::Math {

	template<typename T, typename U, size_t Dim>
	inline Eigen::Vector<U, Dim> Ceil(Eigen::Vector<T, Dim>& vec) {
		Eigen::Vector<U, Dim> r{};
		for (size_t i = 0; i < Dim; ++i)
			r[i] = (U)std::ceil(vec[i]);
		return r;
	}

}