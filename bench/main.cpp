#include <nanobench.h>
#include <module.hpp>
#include <gin/module/registry.hpp>
#include <gin/graph/graph.hpp>
#include <functional>
#include <xmmintrin.h>

class BaseSampler {
public:
    virtual float Get(size_t idx) = 0;
    virtual float operator[](size_t) = 0;
};

class Sampler3D : public BaseSampler {
public:
    Sampler3D(float* arr) : arr{ arr } {};

    float Get(size_t idx) final {
        return arr[idx];
    }

    float operator[](size_t idx) final {
        return arr[idx];
    }
private:
    float* arr{};
};

class SamplerUltimus {
public:
    SamplerUltimus(float* arr) : arr{ arr } {};

    inline float Get(size_t idx) {
        return arr[idx];
    }

    inline float& operator[](size_t idx) {
        return arr[idx];
    }

private:
    float* arr{};
};

struct FunctorSampler {
    float(*Get)(float*,size_t) {};
};

float Getter(float* varr, size_t idx){
     return varr[idx];
}

int main() {
	/*Initialize();

	Gin::Graph::GraphContext ctx{};
	ctx.scale = 1.0;
	ctx.bounds.extent = Eigen::Vector3<double>{64, 64, 64};

	for (auto& entry : Gin::Module::GetNodeRegistry()) {
		Gin::Graph::Graph graph{};
		size_t nodeIdx = entry.second(graph, entry.first);
		graph.GetNode<Gin::Graph::Node>(nodeIdx)->Initialize(ctx);

		ankerl::nanobench::Bench().run(("262144 sample - " + entry.first), [&] {
			graph.GetNode<Gin::Graph::Node>(nodeIdx)->Execute(ctx);
		});
	}

	Uninitialize();*/

    const size_t varrSize = 2621440 * 3;
    float* varr = (float*)malloc(varrSize * sizeof(float));

    for (size_t i = 0; i < varrSize; ++i)
        varr[i] = (float)rand() / 10340.0f;

    BaseSampler* sampler1 = new Sampler3D{ varr };
    SamplerUltimus* samplerUltimus = new SamplerUltimus{ varr };
    FunctorSampler* lambdaSampler = new FunctorSampler{};
    lambdaSampler->Get = Getter;

    ankerl::nanobench::Bench().run(("Normal for loop"), [&] {
        for (size_t i = 0; i < varrSize; ++i) {
            float p = varr[i] * 64 + 2;
            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    ankerl::nanobench::Bench().run(("Unrolled for loop"), [&] {
        for (size_t i = 0; i < varrSize; i += 4) {
            float p[4] = {
                    varr[i] * 64 + 2,
                    varr[i + 1] * 64 + 2,
                    varr[i + 2] * 64 + 2,
                    varr[i + 3] * 64 + 2
            };
            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    ankerl::nanobench::Bench().run(("Normal simd for loop"), [&] {
        for (size_t i = 0; i < varrSize; i += 4) {
            __m128 v1 = _mm_load_ps(&varr[i]);
            __m128 v2 = _mm_set_ps1(64);
            __m128 r = _mm_mul_ps(v1, v2);
            __m128 v3 = _mm_set_ps1(2);

            float v[4]{};
            _mm_store_ps(v, _mm_add_ps(r, v3));

            ankerl::nanobench::doNotOptimizeAway(v);
        }
    });

    printf("\n");

    ankerl::nanobench::Bench().run(("Virtual normal for loop"), [&] {
        for (size_t i = 0; i < varrSize; ++i) {
            float p = sampler1->Get(i) * 64 + 2;
            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    ankerl::nanobench::Bench().run(("Virtual Unrolled for loop"), [&] {
        for (size_t i = 0; i < varrSize; i += 4) {
            float p[4] = {
                    sampler1->Get(i) * 64 + 2,
                    sampler1->Get(i + 1) * 64 + 2,
                    sampler1->Get(i + 2) * 64 + 2,
                    sampler1->Get(i + 3) * 64 + 2
            };
            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    ankerl::nanobench::Bench().run(("Virtual simd for loop"), [&] {
        for (size_t i = 0; i < varrSize; i += 4) {
            __m128 v1 = _mm_set_ps(sampler1->Get(i), sampler1->Get(i + 1), sampler1->Get(i + 2), sampler1->Get(i + 3));
            __m128 v2 = _mm_set_ps1(64);
            __m128 r = _mm_mul_ps(v1, v2);
            __m128 v3 = _mm_set_ps1(2);


            float v[4]{};
            _mm_store_ps(v, _mm_add_ps(r, v3));

            ankerl::nanobench::doNotOptimizeAway(v);
        }
    });

    printf("\n");

    ankerl::nanobench::Bench().run(("Normal for loop (Sphere Signed Distance)"), [&] {
        for (size_t i = 0; i < varrSize / 3; ++i) {
            float v1 = varr[i * 3] * varr[i * 3];
            float v2 = varr[i * 3 + 1] * varr[i * 3 + 1];
            float v3 = varr[i * 3 + 2] * varr[i * 3 + 2];
            float p = std::sqrt(v1 + v2 + v3);

            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    ankerl::nanobench::Bench().run(("Unrolled for loop (Sphere Signed Distance)"), [&] {
        for (size_t i = 0; i < varrSize / 3; ++i) {
            float v1[4] = {
                    varr[i * 3] * varr[i * 3],
                    varr[(i + 1) * 3] * varr[(i + 1) * 3],
                    varr[(i + 2) * 3] * varr[(i + 2) * 3],
                    varr[(i + 3) * 3] * varr[(i + 3) * 3],
            };
            float v2[4] = {
                    varr[i * 3 + 1] * varr[i * 3 + 1],
                    varr[(i + 1) * 3 + 1] * varr[(i + 1) * 3 + 1],
                    varr[(i + 2) * 3 + 1] * varr[(i + 2) * 3 + 1],
                    varr[(i + 3) * 3 + 1] * varr[(i + 3) * 3 + 1],
            };
            float v3[4] = {
                    varr[i * 3 + 2] * varr[i * 3 + 2],
                    varr[(i + 1) * 3 + 2] * varr[(i + 1) * 3 + 2],
                    varr[(i + 2) * 3 + 2] * varr[(i + 2) * 3 + 2],
                    varr[(i + 3) * 3 + 2] * varr[(i + 3) * 3 + 2],
            };
            float p[4] = {
                    std::sqrt(v1[0] + v2[0] + v3[0]),
                    std::sqrt(v1[0] + v2[0] + v3[0]),
                    std::sqrt(v1[0] + v2[0] + v3[0]),
                    std::sqrt(v1[0] + v2[0] + v3[0])
            };

            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    ankerl::nanobench::Bench().run(("Normal simd for loop (Sphere Signed Distance)"), [&] {
        for (size_t i = 0; i < varrSize / 3; i += 4) {
            __m128 v1 = _mm_set_ps(varr[i * 3],
                                   varr[(i + 1) * 3],
                                   varr[(i + 2) * 3],
                                   varr[(i + 3) * 3]);

            __m128 v2 = _mm_set_ps(varr[i * 3 + 1],
                                   varr[(i + 1) * 3 + 1],
                                   varr[(i + 2) * 3 + 1],
                                   varr[(i + 3) * 3 + 1]);

            __m128 v3 = _mm_set_ps(varr[i * 3 + 2],
                                   varr[(i + 1) * 3 + 2],
                                   varr[(i + 2) * 3 + 2],
                                   varr[(i + 3) * 3 + 2]);

            v1 = _mm_mul_ps(v1, v1);
            v2 = _mm_mul_ps(v2, v2);
            v3 = _mm_mul_ps(v3, v3);

            __m128 v4 = _mm_add_ps(_mm_add_ps(v1, v2), v3);
            __m128 r = _mm_sqrt_ps(v4);
            float p[4]{};
            _mm_store_ps(p, r);

            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    free(varr);
}
