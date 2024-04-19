#include "hash.hpp"


Gin::Module::Math::HashVector3::HashVector3() {
    AddInputPort("In", in);

    AddOutputPort("Out", out);
}

void Gin::Module::Math::HashVector3::Initialize(Gin::Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    out.SetField(std::make_shared<Field::ScalarField<Gin::Math::Scalar>>(size.x, size.y, size.z));
}

void Gin::Module::Math::HashVector3::Execute(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::ScalarField<Gin::Math::Scalar>> o = out.GetField<Field::ScalarField<Gin::Math::Scalar>>();

    constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSizeW) {
                auto i = in.GetVector3Batch(x, y, z);

                xsimd::batch<int> xv = xsimd::to_int(i.c[0]);
                xsimd::batch<int> yv = xsimd::to_int(i.c[1]);
                xsimd::batch<int> zv = xsimd::to_int(i.c[2]);

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

                i.c[0] = xsimd::to_float(xv);

                xsimd::store_aligned(&(*o)[idx], i.c[0]);

                /*
                    Eigen::Vector3<unsigned int> u{ (unsigned int)in[idx].x(), (unsigned int)in[idx].y(), (unsigned int)in[idx].z() };

                    u = (u * 1664525u).unaryExpr([&](unsigned int x) { return x + 1013904223u; });

                    u.x() += u.y() * u.z();
                    u.y() += u.x() * u.z();
                    u.z() += u.y() * u.x();

                    u = u.unaryExpr([&](unsigned int x) { return x ^ (x >> 16u); });

                    u.x() += u.y() * u.z();
                    u.y() += u.x() * u.z();
                    u.z() += u.y() * u.x();

                    out[idx] = (u.x() ^ u.y() << 2 ^ u.z() >> 2);
                 * */

                idx += simdSizeW;
            }
        }
    }
}

std::string Gin::Module::Math::HashVector3::GetName() {
    return "Hash Vector3";
}

Gin::Module::Math::UniformDistribution::UniformDistribution() {
    AddInputPort("In", in);
    AddInputPort("Min", min);
    AddInputPort("Max", max);

    AddOutputPort("Out", out);
}

void Gin::Module::Math::UniformDistribution::Initialize(Gin::Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    out.SetField(std::make_shared<Field::ScalarField<Gin::Math::Scalar>>(size.x, size.y, size.z));
}

void Gin::Module::Math::UniformDistribution::Execute(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::ScalarField<Gin::Math::Scalar>> o = out.GetField<Field::ScalarField<Gin::Math::Scalar>>();

    constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSizeW) {
                auto i = in.GetScalarBatch(x, y, z);
                auto m = min.GetScalarBatch(x, y, z);
                auto M = max.GetScalarBatch(x, y, z);

                i = (xsimd::abs(i) / std::numeric_limits<int>::max()) * (M - m) + m;

                xsimd::store_aligned(&(*o)[idx], i);

                idx += simdSizeW;
            }
        }
    }
}

std::string Gin::Module::Math::UniformDistribution::GetName() {
    return "Uniform Distribution";
}

Gin::Module::Math::RandomOffset::RandomOffset() {
    AddInputPort("In", in);
    AddInputPort("Min", min);
    AddInputPort("Max", max);

    AddOutputPort("Out", out);
}

void Gin::Module::Math::RandomOffset::Initialize(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    out.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
}

void Gin::Module::Math::RandomOffset::Execute(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::VectorizedVector3Field> o = out.GetField<Field::VectorizedVector3Field>();

    constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSizeW) {
                auto ix = in.GetScalarBatch(x, y, z);
                auto m = min.GetVector3Batch(x, y, z);
                auto M = max.GetVector3Batch(x, y, z);

                auto iy = ix + ix;
                auto iz = iy + ix;

                ix = (xsimd::abs(ix) / std::numeric_limits<int>::max()) * (M.c[0] - m.c[0]) + m.c[0];
                iy = (xsimd::abs(iy) / std::numeric_limits<int>::max()) * (M.c[1] - m.c[1]) + m.c[1];
                iz = (xsimd::abs(iz) / std::numeric_limits<int>::max()) * (M.c[2] - m.c[2]) + m.c[2];

                Field::VectorizedVector3Field::VectorVector3& vv3 = o->GetVectorVector3(idx);
                xsimd::store_aligned(vv3.x, ix);
                xsimd::store_aligned(vv3.y, iy);
                xsimd::store_aligned(vv3.z, iz);

                ++idx;
            }
        }
    }
}

std::string Gin::Module::Math::RandomOffset::GetName() {
    return "Random Offset";
}

/*
Gin::Module::Math::HashNumber::HashNumber()
{
	AddInputPort("In", in);

	AddOutputPort("Hash", out);
}

void Gin::Module::Math::HashNumber::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		out[idx] = (int)(in[idx]);
		out[idx] = ((out[idx] >> 8) ^ out[idx]) * 1103515245;
		out[idx] = ((out[idx] >> 8) ^ out[idx]) * 1103515245;
		out[idx] = ((out[idx] >> 8) ^ out[idx]) * 1103515245;
	});
}

void Gin::Module::Math::HashNumber::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		out[idx] = (int)(in[idx]);
		out[idx] = ((out[idx] >> 8) ^ out[idx]) * 1103515245;
		out[idx] = ((out[idx] >> 8) ^ out[idx]) * 1103515245;
		out[idx] = ((out[idx] >> 8) ^ out[idx]) * 1103515245;
	});
}

std::string Gin::Module::Math::HashNumber::GetName()
{
	return "Hash Number";
}

Gin::Module::Math::HashVector3::HashVector3()
{
	AddInputPort("In", in);

	AddOutputPort("Hash", out);
}

void Gin::Module::Math::HashVector3::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		Eigen::Vector3<unsigned int> u{ (unsigned int)in[idx].x(), (unsigned int)in[idx].y(), (unsigned int)in[idx].z() };

		u = (u * 1664525u).unaryExpr([&](unsigned int x) { return x + 1013904223u; });

		u.x() += u.y() * u.z();
		u.y() += u.x() * u.z();
		u.z() += u.y() * u.x();

		u = u.unaryExpr([&](unsigned int x) { return x ^ (x >> 16u); });

		u.x() += u.y() * u.z();
		u.y() += u.x() * u.z();
		u.z() += u.y() * u.x();

		out[idx] = (u.x() ^ u.y() << 2 ^ u.z() >> 2);
	});
}

void Gin::Module::Math::HashVector3::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		Eigen::Vector3<unsigned int> u{ (unsigned int)in[idx].x(), (unsigned int)in[idx].y(), (unsigned int)in[idx].z() };

		u = (u * 1664525u).unaryExpr([&](unsigned int x) { return x + 1013904223u; });

		u.x() += u.y() * u.z();
		u.y() += u.x() * u.z();
		u.z() += u.y() * u.x();

		u = u.unaryExpr([&](unsigned int x) { return x ^ (x >> 16u); });

		u.x() += u.y() * u.z();
		u.y() += u.x() * u.z();
		u.z() += u.y() * u.x();

		out[idx] = (u.x() ^ u.y() << 2 ^ u.z() >> 2);
	});
}

std::string Gin::Module::Math::HashVector3::GetName()
{
	return "Hash Vector3";
}

Gin::Module::Math::UniformDistribution::UniformDistribution()
{
	AddInputPort("Hash", hash);
	AddInputPort("Min", min);
	AddInputPort("Max", max);

	AddOutputPort("Value", out);
}

void Gin::Module::Math::UniformDistribution::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		out[idx] = (((double)std::abs(hash[idx]) / (double)std::numeric_limits<int>::max()) * (max - min)) + min;
	});
}

void Gin::Module::Math::UniformDistribution::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		out[idx] = (((double)std::abs(hash[idx]) / (double)std::numeric_limits<int>::max()) * (max - min)) + min;
	});
}

std::string Gin::Module::Math::UniformDistribution::GetName()
{
	return "Uniform Distribution";
}

Gin::Module::Math::NormalDistribution::NormalDistribution()
{
}

void Gin::Module::Math::NormalDistribution::Execute(Graph::GraphContext ctx)
{
}

std::string Gin::Module::Math::NormalDistribution::GetName()
{
	return "Normal Distribution";
}*/
