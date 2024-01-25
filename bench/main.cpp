#include <nanobench.h>
#include <module.hpp>
#include <gin/module/registry.hpp>
#include <gin/graph/graph.hpp>
#include <functional>
#include <xmmintrin.h>
#include <xsimd/xsimd.hpp>

class BaseField {
public:
    virtual void* GetValue(size_t idx) = 0;
    virtual uint32_t GetCount() = 0;
};

class ArrayField : public BaseField {
public:
    ArrayField(float* arr) : arr{ arr } {};

    void* GetValue(size_t idx) final {
        return &arr[idx];
    }

    uint32_t GetCount() final {
        return 1;
    }
private:
    float* arr{};
};

class Sampler3D {
public:
    Sampler3D(BaseField* f) : f{ f }, c{ f->GetCount() } {};

    inline float Get(size_t idx) {
        switch (c) {
            case 1:
                return *((float*)f->GetValue(idx));
            case 2:
                return *((float*)f->GetValue(idx) + 1);
            case 3:
                return *((float*)f->GetValue(idx) + 2);
            case 4:
                return *((float*)f->GetValue(idx) + 3);
            default:
                return 0;
        }
    }

private:
    BaseField* f{};
    uint32_t c{};
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
	Initialize();

	Gin::Graph::GraphContext ctx{};
	ctx.scale = 1.0;
	ctx.bounds.extent = Gin::Math::Vector3{64, 64, 64};

	for (auto& entry : Gin::Module::GetNodeRegistry()) {
		Gin::Graph::Graph graph{};
		size_t nodeIdx = entry.second(graph, entry.first);
		graph.GetNode<Gin::Graph::Node>(nodeIdx)->Initialize(ctx);

		ankerl::nanobench::Bench().run(("262144 sample - " + entry.first), [&] {
			graph.GetNode<Gin::Graph::Node>(nodeIdx)->Execute(ctx);
		});
	}

    printf("\n");

	Uninitialize();

    const size_t varrSize = 2621440 * 3;
    float* varr = (float*)malloc(varrSize * sizeof(float));

    for (size_t i = 0; i < varrSize; ++i)
        varr[i] = (float)rand() / 10340.0f;

    BaseField* field = new ArrayField{ varr };
    Sampler3D sampler1{ field };
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
        constexpr size_t s = xsimd::simd_type<float>::size;

        for (size_t i = 0; i < varrSize; i += s) {
            auto v1 = xsimd::load_aligned(&varr[i]);
            auto res = v1 * 64 + 2;

            float v[4]{};
            xsimd::store_aligned(v, res);

            ankerl::nanobench::doNotOptimizeAway(v);
        }
    });

    printf("\n");

    ankerl::nanobench::Bench().run(("Virtual normal for loop"), [&] {
        for (size_t i = 0; i < varrSize; ++i) {
            float p = sampler1.Get(i) * 64 + 2;
            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    ankerl::nanobench::Bench().run(("Virtual Unrolled for loop"), [&] {
        for (size_t i = 0; i < varrSize; i += 4) {
            float p[4] = {
                    sampler1.Get(i) * 64 + 2,
                    sampler1.Get(i + 1) * 64 + 2,
                    sampler1.Get(i + 2) * 64 + 2,
                    sampler1.Get(i + 3) * 64 + 2
            };
            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    ankerl::nanobench::Bench().run(("Virtual simd for loop"), [&] {
        for (size_t i = 0; i < varrSize; i += 4) {
            __m128 v1 = _mm_set_ps(sampler1.Get(i), sampler1.Get(i + 1), sampler1.Get(i + 2), sampler1.Get(i + 3));
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


    ankerl::nanobench::Bench().run(("Normal xsimd for loop (Sphere Signed Distance)"), [&] {
        for (size_t i = 0; i < varrSize / 3; i += xsimd::simd_type<float>::size) {

            static float a1[xsimd::simd_type<float>::size] = {};
            static float a2[xsimd::simd_type<float>::size] = {};
            static float a3[xsimd::simd_type<float>::size] = {};

            for (int j = 0; j < xsimd::simd_type<float>::size; ++j) {
                a1[j] = varr[(i + j) * 3];
                a2[j] = varr[(i + j) * 3 + 1];
                a3[j] = varr[(i + j) * 3 + 2];
            }

            auto v1 = xsimd::load_aligned(a1);
            auto v2 = xsimd::load_aligned(a2);
            auto v3 = xsimd::load_aligned(a3);

            auto r = xsimd::sqrt(v1 * v1 + v2 * v2 + v3 * v3);

            float p[xsimd::simd_type<float>::size]{};
            xsimd::store_aligned(p, r);

            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    printf("\n");

    ankerl::nanobench::Bench().run(("Virtual Normal for loop (Sphere Signed Distance)"), [&] {
        for (size_t i = 0; i < varrSize / 3; ++i) {
            float v1 = sampler1.Get(i * 3) * sampler1.Get(i * 3);
            float v2 = sampler1.Get(i * 3 + 1) * sampler1.Get(i * 3 + 1);
            float v3 = sampler1.Get(i * 3 + 2) * sampler1.Get(i * 3 + 2);
            float p = std::sqrt(v1 + v2 + v3);

            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    ankerl::nanobench::Bench().run(("Normal simd for loop (Sphere Signed Distance)"), [&] {
        for (size_t i = 0; i < varrSize / 3; i += 4) {
            __m128 v1 = _mm_set_ps(sampler1.Get(i * 3),
                                   sampler1.Get((i + 1) * 3),
                                   sampler1.Get((i + 2) * 3),
                                   sampler1.Get((i + 3) * 3));

            __m128 v2 = _mm_set_ps(sampler1.Get(i * 3 + 1),
                                   sampler1.Get((i + 1) * 3 + 1),
                                   sampler1.Get((i + 2) * 3 + 1),
                                   sampler1.Get((i + 3) * 3 + 1));

            __m128 v3 = _mm_set_ps(sampler1.Get(i * 3 + 2),
                                   sampler1.Get((i + 1) * 3 + 2),
                                   sampler1.Get((i + 2) * 3 + 2),
                                   sampler1.Get((i + 3) * 3 + 2));

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

    ankerl::nanobench::Bench().run(("Virtual Normal xsimd for loop (Sphere Signed Distance)"), [&] {
        for (size_t i = 0; i < varrSize / 3; i += xsimd::simd_type<float>::size) {

            static float a1[xsimd::simd_type<float>::size] = {};
            static float a2[xsimd::simd_type<float>::size] = {};
            static float a3[xsimd::simd_type<float>::size] = {};

            for (int j = 0; j < xsimd::simd_type<float>::size; ++j) {
                a1[j] = sampler1.Get((i + j) * 3);
                a2[j] = sampler1.Get((i + j) * 3 + 1);
                a3[j] = sampler1.Get((i + j) * 3 + 2);
            }

            auto v1 = xsimd::load_aligned(a1);
            auto v2 = xsimd::load_aligned(a2);
            auto v3 = xsimd::load_aligned(a3);

            auto r = xsimd::sqrt(v1 * v1 + v2 * v2 + v3 * v3);

            float p[xsimd::simd_type<float>::size]{};
            xsimd::store_aligned(p, r);

            ankerl::nanobench::doNotOptimizeAway(p);
        }
    });

    free(varr);
}
