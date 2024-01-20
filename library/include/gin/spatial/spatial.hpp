#pragma once

#include <memory>
#include <gin/profiler/profiler.hpp>

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
            Profiler::RecordAllocation(1 * sizeof(T), data.get());
		}
		Spatial(T v) : size{ 1, 0, 0 } {
			data = std::shared_ptr<T[]>(new T[1]);
			data[0] = v;
            Profiler::RecordAllocation(1 * sizeof(T), data.get());
		}

		inline size_t GetWidth() { return size[0]; }
		inline size_t GetHeight() { return size[1]; }
		inline size_t GetDepth() { return size[2]; }

		virtual void Resize(size_t width, size_t height, size_t depth) final {
			if (size[0] == width && size[1] == height && size[2] == depth)
				return;

            Profiler::RecordDeallocation(data.get());

			size[0] = width;
			size[1] = height;
			size[2] = depth;

			T v = data[0];

			data = std::shared_ptr<T[]>(new T[width * height * depth]);
            Profiler::RecordAllocation(width * height * depth * sizeof(T), data.get());
			std::fill(&data[0], &data[width * height * depth], v);
		}

		inline T* Data() {
			return data.get();
		}

		inline T& operator[](size_t idx) {
			return data[idx];
		}

	private:
		size_t size[3];
		std::shared_ptr<T[]> data{};
	};

}