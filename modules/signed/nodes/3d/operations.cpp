#include <signed/nodes/3d/operations.hpp>
#include <gin/math/math.hpp>


Gin::Module::Signed::OPUnion::OPUnion()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);

	AddOutputPort("Signed Distance", distanceR);
	AddOutputPort("T", t);
}

void Gin::Module::Signed::OPUnion::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    distanceR.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
    t.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Signed::OPUnion::Execute(Graph::GraphContext ctx)
{
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::ScalarField<float>> fR = distanceR.GetField<Field::ScalarField<float>>();
    std::shared_ptr<Field::ScalarField<float>> fT = t.GetField<Field::ScalarField<float>>();

    constexpr size_t simdSize = xsimd::simd_type<Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSize) {
                xsimd::batch<float> dA = distanceA.GetScalarBatch(x, y, z);
                xsimd::batch<float> dB = distanceB.GetScalarBatch(x, y, z);

                xsimd::store_aligned(&(*fR)[idx], xsimd::min(dA, dB));
                xsimd::store_aligned(&(*fT)[idx], xsimd::incr_if(xsimd::batch<float>{ 0.0f }, dA > dB));

                idx += simdSize;
            }
        }
    }
}

std::string Gin::Module::Signed::OPUnion::GetName()
{
	return "OPUnion";
}

Gin::Module::Signed::OPSubstraction::OPSubstraction()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);

	AddOutputPort("Signed Distance", distanceR);
	AddOutputPort("T", t);
}

void Gin::Module::Signed::OPSubstraction::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    distanceR.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
    t.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Signed::OPSubstraction::Execute(Graph::GraphContext ctx)
{
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::ScalarField<float>> fR = distanceR.GetField<Field::ScalarField<float>>();
    std::shared_ptr<Field::ScalarField<float>> fT = t.GetField<Field::ScalarField<float>>();

    constexpr size_t simdSize = xsimd::simd_type<Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSize) {
                xsimd::batch<float> dA = distanceA.GetScalarBatch(x, y, z);
                xsimd::batch<float> dB = distanceB.GetScalarBatch(x, y, z);

                xsimd::store_aligned(&(*fR)[idx], xsimd::max(-dA, dB));
                xsimd::store_aligned(&(*fT)[idx], xsimd::decr_if(xsimd::batch<float>{ 1.0f }, -dA > dB));

                idx += simdSize;
            }
        }
    }
}

std::string Gin::Module::Signed::OPSubstraction::GetName()
{
	return "OPSubstraction";
}

Gin::Module::Signed::OPIntersection::OPIntersection()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);

	AddOutputPort("Signed Distance", distanceR);
	AddOutputPort("T", t);
}

void Gin::Module::Signed::OPIntersection::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    distanceR.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
    t.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Signed::OPIntersection::Execute(Graph::GraphContext ctx)
{
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::ScalarField<float>> fR = distanceR.GetField<Field::ScalarField<float>>();
    std::shared_ptr<Field::ScalarField<float>> fT = t.GetField<Field::ScalarField<float>>();

    constexpr size_t simdSize = xsimd::simd_type<Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSize) {
                xsimd::batch<float> dA = distanceA.GetScalarBatch(x, y, z);
                xsimd::batch<float> dB = distanceB.GetScalarBatch(x, y, z);

                xsimd::store_aligned(&(*fR)[idx], xsimd::max(dA, dB));
                xsimd::store_aligned(&(*fT)[idx], xsimd::decr_if(xsimd::batch<float>{ 1.0f }, dA > dB));

                idx += simdSize;
            }
        }
    }
}

std::string Gin::Module::Signed::OPIntersection::GetName()
{
	return "OPIntersection";
}

Gin::Module::Signed::OPSmoothUnion::OPSmoothUnion()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);
	AddInputPort("Smooth Factor", smoothFactor);

	AddOutputPort("Signed Distance", distanceR);
	AddOutputPort("T", t);
}

void Gin::Module::Signed::OPSmoothUnion::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    distanceR.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
    t.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Signed::OPSmoothUnion::Execute(Graph::GraphContext ctx)
{
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::ScalarField<float>> fR = distanceR.GetField<Field::ScalarField<float>>();
    std::shared_ptr<Field::ScalarField<float>> fT = t.GetField<Field::ScalarField<float>>();

    constexpr size_t simdSize = xsimd::simd_type<Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSize) {
                xsimd::batch<float> dA = distanceA.GetScalarBatch(x, y, z);
                xsimd::batch<float> dB = distanceB.GetScalarBatch(x, y, z);
                xsimd::batch<float> k = smoothFactor.GetScalarBatch(x, y, z);

                xsimd::batch<float> h = xsimd::max(k - xsimd::abs(dA - dB), xsimd::batch<float>{ 0.0f }) / k;
                xsimd::batch<float> m = h * h * 0.5f;
                xsimd::batch<float> s = m * k * 0.5f;

                xsimd::batch<float> f = xsimd::incr_if(xsimd::batch<float>{ 0.0f }, dA > dB);

                xsimd::batch<float> v1 = (dA - s) * (1.0f - f) + (dB - s) * f;
                xsimd::batch<float> v2 = f - m;

                xsimd::store_aligned(&(*fR)[idx], v1);
                xsimd::store_aligned(&(*fT)[idx], v2);

                idx += simdSize;
            }
        }
    }
}

std::string Gin::Module::Signed::OPSmoothUnion::GetName()
{
	return "OPSmoothUnion";
}

Gin::Module::Signed::OPSmoothSubstraction::OPSmoothSubstraction()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);
	AddInputPort("Smooth Factor", smoothFactor);

	AddOutputPort("Signed Distance", distanceR);
	AddOutputPort("T", t);
}

void Gin::Module::Signed::OPSmoothSubstraction::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    distanceR.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
    t.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Signed::OPSmoothSubstraction::Execute(Graph::GraphContext ctx)
{
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::ScalarField<float>> fR = distanceR.GetField<Field::ScalarField<float>>();
    std::shared_ptr<Field::ScalarField<float>> fT = t.GetField<Field::ScalarField<float>>();

    constexpr size_t simdSize = xsimd::simd_type<Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSize) {
                xsimd::batch<float> dA = -distanceA.GetScalarBatch(x, y, z);
                xsimd::batch<float> dB = distanceB.GetScalarBatch(x, y, z);
                xsimd::batch<float> k = smoothFactor.GetScalarBatch(x, y, z);

                xsimd::batch<float> h = xsimd::max(k - xsimd::abs(dA - dB), xsimd::batch<float>{ 0.0f }) / k;
                xsimd::batch<float> m = h * h * 0.5f;
                xsimd::batch<float> s = m * k * 0.5f;

                xsimd::batch<float> f = xsimd::incr_if(xsimd::batch<float>{ 0.0f }, dA > dB);

                xsimd::batch<float> v1 = (dA - s) * (1.0f - f) + (dB - s) * f;
                xsimd::batch<float> v2 = f - m;

                xsimd::store_aligned(&(*fR)[idx], v1);
                xsimd::store_aligned(&(*fT)[idx], v2);

                idx += simdSize;
            }
        }
    }
}

std::string Gin::Module::Signed::OPSmoothSubstraction::GetName()
{
	return "OPSmoothSubstraction";
}
/*
Gin::Module::Signed::OPSmoothIntersection::OPSmoothIntersection()
{
	AddInputPort("A", distanceA);
	AddInputPort("B", distanceB);
	AddInputPort("Smooth Factor", smoothFactor);

	AddOutputPort("Signed Distance", distanceR);
	AddOutputPort("T", t);
}

void Gin::Module::Signed::OPSmoothIntersection::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		Eigen::Vector2<float> r = Math::SMax(distanceA[idx], distanceB[idx], smoothFactor[idx]);
		distanceR[idx] = r.x();
		t[idx] = r.y();
	});
}

void Gin::Module::Signed::OPSmoothIntersection::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t x, size_t y, size_t z) {
		Eigen::Vector2<float> r = Math::SMax(distanceA[idx], distanceB[idx], smoothFactor[idx]);
		distanceR[idx] = r.x();
		t[idx] = r.y();
	});
}

std::string Gin::Module::Signed::OPSmoothIntersection::GetName()
{
	return "OPSmoothIntersection";
}*/