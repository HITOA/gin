#pragma once

#include <memory>

namespace Gin::Spatial {

	class BaseSpatial {
	public:
		virtual ~BaseSpatial() {};

		virtual void Resize(size_t width, size_t height, size_t depth) = 0;
	};

	template<typename T>
	class Spatial : BaseSpatial {
	public:
		Spatial() : size{ 1, 0, 0 } {
			data = std::shared_ptr<T[]>(new T[1]);
			data[0] = T{};
		}
		Spatial(T v) : size{ 1, 0, 0 } {
			data = std::shared_ptr<T[]>(new T[1]);
			data[0] = v;
		}

		virtual void Resize(size_t width, size_t height, size_t depth) final {
			if (size[0] == width && size[1] == height && size[2] == depth)
				return;

			size[0] = width;
			size[1] = height;
			size[2] = depth;

			T v = data[0];

			data = std::shared_ptr<T[]>(new T[width * height * depth]);
			std::fill(&data[0], &data[width * height * depth], v);
		}

		inline T& operator[](size_t idx) {
			return data[idx];
		}

	private:
		size_t size[3];
		std::shared_ptr<T[]> data{};
	};

}