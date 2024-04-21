//
// Created by HITO on 20/04/24.
//

#include "point.hpp"

Gin::Math::Vector3 GetRandomOffsetFromPosition(Gin::Math::Vector3 position, Gin::Math::Vector3 min, Gin::Math::Vector3 max) {
    uint32_t xv = position.x;
    uint32_t yv = position.y;
    uint32_t zv = position.z;

    xv = (xv * 1664525U) + 1013904223U;
    yv = (yv * 1664525U) + 1013904223U;
    zv = (zv * 1664525U) + 1013904223U;

    xv += yv * zv;
    yv += xv * zv;
    zv += yv * xv;

    xv = xv ^ (xv >> 16U);
    yv = yv ^ (yv >> 16U);
    zv = zv ^ (zv >> 16U);

    xv += yv * zv;
    yv += xv * zv;
    zv += yv * xv;

    xv = xv ^ yv << 2 ^ zv >> 2;

    yv = ((xv >> 8) ^ xv) * 1103515245U;
    yv = ((yv >> 8) ^ yv) * 1103515245U;
    yv = ((yv >> 8) ^ yv) * 1103515245U;

    zv = ((yv >> 8) ^ yv) * 1103515245U;
    zv = ((zv >> 8) ^ zv) * 1103515245U;
    zv = ((zv >> 8) ^ zv) * 1103515245U;

    float x = ((float)xv / (float)std::numeric_limits<int>::max()) * (max.x - min.x) + min.x;
    float y = ((float)yv / (float)std::numeric_limits<int>::max()) * (max.y - min.y) + min.y;
    float z = ((float)zv / (float)std::numeric_limits<int>::max()) * (max.z - min.z) + min.z;

    return Gin::Math::Vector3{ x, y, z };
}

Gin::Module::Geometry::PointsCloud::PointsCloud() {
    AddInputPort("Distance", distance);
    AddInputPort("Randomness", randomness);

    AddOutputPort("Points", points);
}

void Gin::Module::Geometry::PointsCloud::Initialize(Graph::GraphContext ctx) {
    points = std::make_shared<std::vector<Gin::Math::Vector3>>();
}

void Gin::Module::Geometry::PointsCloud::Execute(Graph::GraphContext ctx) {
    Math::Vector3 size = Math::Floor((ctx.bounds.extent * 2) / distance);
    Math::Vector3 offset = ctx.bounds.origin;
    offset.x = fmod(offset.x, distance) / distance;
    offset.y = fmod(offset.y, distance) / distance;
    offset.z = fmod(offset.z, distance) / distance;

    points->resize(size.x * size.y * size.z);

    Math::Vector3 max{ randomness, randomness, randomness };
    Math::Vector3 min = Math::Vector3{ 0.0f } - max;

    for (int z = 0; z < size.z; ++z) {
        for (int y = 0; y < size.y; ++y) {
            for (int x = 0; x < size.x; ++x) {
                Math::Vector3 currentPoint{ (float)x, (float)y, (float)z };
                currentPoint -= size / 2.0;
                currentPoint *= distance;
                currentPoint += ctx.bounds.origin - offset + distance / 2;
                currentPoint += GetRandomOffsetFromPosition(currentPoint, min, max);
                (*points)[x + y * (int)size.x + z * (int)size.x * (int)size.y] = currentPoint;
            }
        }
    }
}

std::string Gin::Module::Geometry::PointsCloud::GetName() {
    return "Points Cloud";
}

Gin::Module::Geometry::ScatterSampler::ScatterSampler() {
    AddInputPort("In", primitive);
    AddInputPort("Points", points);
    AddInputPort("Bounding Sphere Radius", boudingSphereRadius);
    AddInputPort("Smoothness", smoothness);

    AddOutputPort("Out", distance);
}

void Gin::Module::Geometry::ScatterSampler::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    distance.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Geometry::ScatterSampler::Execute(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    Field::Sampler<Math::Vector3> position{};
    position.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));

    {
        std::shared_ptr<Field::VectorizedVector3Field> f = position.GetField<Field::VectorizedVector3Field>();

        constexpr size_t simdSizeW = xsimd::simd_type<Math::Scalar>::size;
        size_t idx = 0;
        static float xcOffsets[simdSizeW]{};
        for (size_t i = 0; i < simdSizeW; ++i)
            xcOffsets[i] = i;

        for (size_t z = 0; z < size.z; ++z) {
            for (size_t y = 0; y < size.y; ++y) {
                for (size_t x = 0; x < size.x; x += simdSizeW) {
                    xsimd::batch<Math::Scalar> xc = xsimd::load_unaligned(xcOffsets) + x;
                    xsimd::batch<Math::Scalar> yc{ (float)y };
                    xsimd::batch<Math::Scalar> zc{ (float)z };

                    xc = xc * ctx.scale - ctx.bounds.extent.x;
                    yc = yc * ctx.scale - ctx.bounds.extent.y;
                    zc = zc * ctx.scale - ctx.bounds.extent.z;

                    Field::VectorizedVector3Field::VectorVector3& vv3 = f->GetVectorVector3(idx);
                    xsimd::store_aligned(vv3.x, xc);
                    xsimd::store_aligned(vv3.y, yc);
                    xsimd::store_aligned(vv3.z, zc);

                    ++idx;
                }
            }
        }
    }

    Field::Sampler<float> sdf = primitive->Compute(position);
    position.Clear();


    std::shared_ptr<Field::ScalarField<float>> d = distance.GetField<Field::ScalarField<float>>();
    d->Fill(boudingSphereRadius);

    int radius = std::ceil(boudingSphereRadius / ctx.scale);

    for (int i = 0; i < points->size(); ++i) {
        Math::Vector3 point = (*points)[i];
        point = (point - ctx.bounds.origin + ctx.bounds.extent) / ctx.scale;
        for (int z = -radius; z <= radius; ++z) {
            for (int y = -radius; y <= radius; ++y) {
                for (int x = -radius; x <= radius; ++x) {
                    Math::Vector3 sPoint = Math::Round(point + Math::Vector3{ (float)x, (float)y, (float)z });
                    if (sPoint.x >= 0 && sPoint.y >= 0 && sPoint.z >= 0 &&
                            sPoint.x < size.x && sPoint.y < size.y && sPoint.z < size.z) {
                        float v = sdf.GetScalar(x + size.x / 2, y + size.y / 2, z + size.z / 2);
                        size_t idx = (int)sPoint.x + (int)sPoint.y * d->GetVecWidth() + (int)sPoint.z * d->GetVecWidth() * (int)size.y;
                        float ov = (*d)[idx];

                        float h = std::max(smoothness - std::abs(v - ov), 0.0f) / smoothness;
                        float m = h * h * 0.5f;
                        float s = m * smoothness * 0.5f;

                        (*d)[idx] = v < ov ? v - s : ov - s;
                    }
                }
            }
        }
    }
}

std::string Gin::Module::Geometry::ScatterSampler::GetName() {
    return "Scatter Sampler";
}