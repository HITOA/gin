//
// Created by HITO on 03/05/24.
//

#include "noises.hpp"

Gin::Module::Signed::SDSphericalNoise::SDSphericalNoise() {
    AddOutputPort("Out", out);
}

void Gin::Module::Signed::SDSphericalNoise::Initialize(Gin::Graph::GraphContext ctx) {
    out = std::make_shared<SphericalNoise>();
}

void Gin::Module::Signed::SDSphericalNoise::Execute(Gin::Graph::GraphContext ctx) {}

std::string Gin::Module::Signed::SDSphericalNoise::GetName() {
    return "SDSphericalNoise";
}

Gin::Field::Sampler<float>
Gin::Module::Signed::SDSphericalNoise::SphericalNoise::Compute(Field::Sampler<Math::Vector3> position,
                                                               float frequency, int seed) {
    Math::Vector3 size{ 1.0f };

    if (position.IsFieldOfType<Field::VectorizedVector3Field>()) {
        std::shared_ptr<Field::VectorizedVector3Field> p = position.GetField<Field::VectorizedVector3Field>();
        size.x = p->GetWidth();
        size.y = p->GetHeight();
        size.z = p->GetDepth();
    }

    std::shared_ptr<Field::ScalarField<float>> d = std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z);
    Field::Sampler<float> s{};
    s.SetField(d);

    constexpr size_t simdSize = xsimd::simd_type<Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSize) {
                auto pos = position.GetVector3Batch(x, y, z);

                pos.c[0] *= frequency;
                pos.c[1] *= frequency;
                pos.c[2] *= frequency;

                xsimd::batch<float> ix = xsimd::floor(pos.c[0]);
                xsimd::batch<float> iy = xsimd::floor(pos.c[1]);
                xsimd::batch<float> iz = xsimd::floor(pos.c[2]);

                pos.c[0] -= ix;
                pos.c[1] -= iy;
                pos.c[2] -= iz;

                xsimd::batch<float> r{ 10.0 };

                for (int z = 0; z < 2; ++z) {
                    for (int y = 0; y < 2; ++y) {
                        for (int x = 0; x < 2; ++x) {
                            xsimd::batch<int> xv = xsimd::to_int(ix) + x + seed;
                            xsimd::batch<int> yv = xsimd::to_int(iy) + y + seed;
                            xsimd::batch<int> zv = xsimd::to_int(iz) + z + seed;

                            //xv = (xv * 73856093U) ^ (yv * 19349663U) ^ (zv * 83492791U);
                            //yv = (xv * 805306457U) ^ (yv * 402653189U) ^ (zv * 201326611U);
                            //zv = (xv * 100663319U) ^ (yv * 50331653U) ^ (zv * 1610612741U);

                            xv = (xv * 1664525) + 1013904223;
                            yv = (yv * 1664525) + 1013904223;
                            zv = (zv * 1664525) + 1013904223;

                            xv += yv * zv;
                            yv += xv * zv;
                            zv += yv * xv;

                            xv = xv ^ (xv >> 16);
                            yv = yv ^ (yv >> 16);
                            zv = zv ^ (zv >> 16);

                            xv += yv * zv;
                            yv += xv * zv;
                            zv += yv * xv;

                            xv = xv ^ yv << 2 ^ zv >> 2;
                            yv = xv + zv;
                            zv = xv + yv;

                            auto radius = (xsimd::to_float(xv + yv + zv) / (float)std::numeric_limits<int>::max()) * (0.5f - 0.1f) + 0.1f;

                            auto xOffset = (xsimd::to_float(xv) / (float)std::numeric_limits<int>::max() - 0.5f) * 0.2f;
                            auto yOffset = (xsimd::to_float(yv) / (float)std::numeric_limits<int>::max() - 0.5f) * 0.2f;
                            auto zOffset = (xsimd::to_float(zv) / (float)std::numeric_limits<int>::max() - 0.5f) * 0.2f;

                            auto cx = pos.c[0] - x + xOffset;
                            auto cy = pos.c[1] - y + yOffset;
                            auto cz = pos.c[2] - z + zOffset;
                            auto cd = xsimd::sqrt(cx * cx + cy * cy + cz * cz) - radius;

                            r = xsimd::min(r, cd);
                        }
                    }
                }

                xsimd::store_aligned(&(*d)[idx], r);
                idx += simdSize;
            }
        }
    }

    return s;
}

Gin::Module::Signed::SDFBMNoiseSampler::SDFBMNoiseSampler() {
    AddInputPort("In", in);
    AddInputPort("Position", position);
    AddInputPort("Host", distance);
    AddInputPort("Frequency", frequency);
    AddInputPort("Seed", seed);
    AddInputPort("Octave", octaves);
    AddInputPort("Gain", gain);
    AddInputPort("Lacunarity", lacunarity);

    AddOutputPort("Distance", out);
}

void Gin::Module::Signed::SDFBMNoiseSampler::Initialize(Gin::Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    out.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Signed::SDFBMNoiseSampler::Execute(Gin::Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::vector<Field::Sampler<float>> noises{};
    noises.resize(octaves);
    for (int i = 0; i < octaves; ++i) {
        noises[i] = in->Compute(position, frequency * std::pow(lacunarity, i), ((int)seed << i) + (int)seed);
    }

    std::shared_ptr<Field::ScalarField<float>> d = out.GetField<Field::ScalarField<float>>();

    constexpr size_t simdSize = xsimd::simd_type<Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSize) {
                auto hostDistance = distance.GetScalarBatch(x, y, z);

                float s = 1.0f / frequency;

                for (int i = 0; i < octaves; ++i) {
                    auto inflatedHostDistance = hostDistance - 0.1f * s;
                    auto noiseDistance = s * noises[i].GetScalarBatch(x, y, z);
                    float k = 0.3f * s;

                    auto h = xsimd::max(k - xsimd::abs(noiseDistance - inflatedHostDistance), xsimd::batch<float>{ 0.0f });
                    noiseDistance = xsimd::max(noiseDistance, inflatedHostDistance) + h * h * 0.25 / k;

                    h = xsimd::max(k - xsimd::abs(noiseDistance - hostDistance), xsimd::batch<float>{ 0.0f });
                    hostDistance = xsimd::min(noiseDistance, hostDistance) - h * h * 0.25 / k;
                    s *= gain;
                }

                xsimd::store_aligned(&(*d)[idx], hostDistance);

                idx += simdSize;
            }
        }
    }
}


void Gin::Module::Signed::SDFBMNoiseSampler::Execute(Gin::Graph::GraphContext ctx, Thread::ThreadPool& pool) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::vector<Field::Sampler<float>> noises{};
    noises.resize(octaves);
    unsigned int running{ 0 };
    std::mutex mutex{};
    std::condition_variable cv{};

    for (int i = 0; i < octaves; ++i) {
        ++running;
        pool.Execute([this, &noises, i, &mutex, &cv, &running]() {
            noises[i] = in->Compute(position, frequency * std::pow(lacunarity, i), ((int)seed << i) + (int)seed);
            {
                std::unique_lock<std::mutex> lock{ mutex };
                --running;
                cv.notify_all();
            }
        });
    }

    while (running) {
        std::unique_lock<std::mutex> lock{ mutex };
        cv.wait(lock, [&] {
            return running <= 0;
        });
    }

    std::shared_ptr<Field::ScalarField<float>> d = out.GetField<Field::ScalarField<float>>();

    constexpr size_t simdSize = xsimd::simd_type<Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSize) {
                auto hostDistance = distance.GetScalarBatch(x, y, z);

                float s = 1.0f / frequency;

                for (int i = 0; i < octaves; ++i) {
                    auto inflatedHostDistance = hostDistance - 0.1f * s;
                    auto noiseDistance = s * noises[i].GetScalarBatch(x, y, z);
                    float k = 0.3f * s;

                    auto h = xsimd::max(k - xsimd::abs(noiseDistance - inflatedHostDistance), xsimd::batch<float>{ 0.0f });
                    noiseDistance = xsimd::max(noiseDistance, inflatedHostDistance) + h * h * 0.25 / k;

                    h = xsimd::max(k - xsimd::abs(noiseDistance - hostDistance), xsimd::batch<float>{ 0.0f });
                    hostDistance = xsimd::min(noiseDistance, hostDistance) - h * h * 0.25 / k;
                    s *= gain;
                }

                xsimd::store_aligned(&(*d)[idx], hostDistance);

                idx += simdSize;
            }
        }
    }
}

std::string Gin::Module::Signed::SDFBMNoiseSampler::GetName() {
    return "SDFBMNoiseSampler";
}