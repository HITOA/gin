#include "primitives.hpp"

Gin::Module::Signed::SDSphere::SDSphere()
{
	AddInputPort("Radius", radius);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Module::Signed::SDSphere::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ ctx.bounds.extent * 2.0 / ctx.scale };
    size.x = std::ceil(size.x);
    size.y = std::ceil(size.y);
    size.z = std::ceil(size.z);

    distance.SetField(std::make_shared<Field::ScalarField<float>>(size.x, size.y, size.z));
}

void Gin::Module::Signed::SDSphere::Execute(Graph::GraphContext ctx)
{
    Math::Vector3 size{ ctx.bounds.extent * 2.0 / ctx.scale };
    size.x = std::ceil(size.x);
    size.y = std::ceil(size.y);
    size.z = std::ceil(size.z);

    std::shared_ptr<Field::ScalarField<float>> d = distance.GetField<Field::ScalarField<float>>();

    constexpr size_t simdSize = xsimd::simd_type<Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSize) {
                auto pos = position.GetVector3Batch(x, y, z);
                auto rad = radius.GetScalarBatch(x, y, z);

                for (auto& i : pos.c)
                    i = i * i;

                auto r = xsimd::sqrt(pos.c[0] + pos.c[1] + pos.c[2]) - rad;
                xsimd::store_aligned(&(*d)[idx], r);
                idx += simdSize;
            }
        }
    }
}

std::string Gin::Module::Signed::SDSphere::GetName()
{
	return "SDSphere";
}
/*
Gin::Module::Signed::SDBox::SDBox()
{
	AddInputPort("Size", size);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Module::Signed::SDBox::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		Eigen::Vector3<double> q = position[idx].cwiseAbs() - size[idx];
		Eigen::Vector3<double> g = q;
		g.x() = g.x() > 0.0 ? g.x() : 0.0;
		g.y() = g.y() > 0.0 ? g.y() : 0.0;
		g.z() = g.z() > 0.0 ? g.z() : 0.0;
		distance[idx] = (float)(sqrt(g.dot(g)) + Math::Min<double>(Math::Max<double>(q.x(), Math::Max<double>(q.y(), q.z())), 0.0));
	});
}

void Gin::Module::Signed::SDBox::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t x, size_t y, size_t z) {
		Eigen::Vector3<double> q = position[idx].cwiseAbs() - size[idx];
		Eigen::Vector3<double> g = q;
		g.x() = g.x() > 0.0 ? g.x() : 0.0;
		g.y() = g.y() > 0.0 ? g.y() : 0.0;
		g.z() = g.z() > 0.0 ? g.z() : 0.0;
		distance[idx] = (float)(sqrt(g.dot(g)) + Math::Min<double>(Math::Max<double>(q.x(), Math::Max<double>(q.y(), q.z())), 0.0));
	});
}

std::string Gin::Module::Signed::SDBox::GetName()
{
	return "SDBox";
}


Gin::Module::Signed::SDGround::SDGround()
{
	AddInputPort("Height", height);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Module::Signed::SDGround::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distance[idx] = position[idx].y() - height[idx];
	});
}

void Gin::Module::Signed::SDGround::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t x, size_t y, size_t z) {
		distance[idx] = position[idx].y() - height[idx];
	});
}

std::string Gin::Module::Signed::SDGround::GetName()
{
	return "SDGround";
}

Gin::Module::Signed::SDPlane::SDPlane()
{
	AddInputPort("Height", height);
	AddInputPort("Normal", normal);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Module::Signed::SDPlane::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distance[idx] = position[idx].dot(normal[idx]) + height[idx];
	});
}

void Gin::Module::Signed::SDPlane::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t x, size_t y, size_t z) {
		distance[idx] = position[idx].dot(normal[idx]) + height[idx];
	});
}

std::string Gin::Module::Signed::SDPlane::GetName()
{
	return "SDPlane";
}

Gin::Module::Signed::SDTorus::SDTorus()
{
	AddInputPort("Inner Radius", innerRadius);
	AddInputPort("Outter Radius", outterRadius);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Module::Signed::SDTorus::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		Eigen::Vector2<float> t{ innerRadius[idx], outterRadius[idx] };
		Eigen::Vector2<float> p{ position[idx].x(), position[idx].z() };
		Eigen::Vector2<float> q{ p.dot(p) - t.x(), position[idx].y() };
		
		distance[idx] = q.dot(q) - t.y();
	});
}

void Gin::Module::Signed::SDTorus::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t x, size_t y, size_t z) {
		Eigen::Vector2<float> t{ innerRadius[idx], outterRadius[idx] };
		Eigen::Vector2<float> p{ position[idx].x(), position[idx].z() };
		Eigen::Vector2<float> q{ sqrt(p.dot(p)) - t.x(), position[idx].y() };

		distance[idx] = sqrt(q.dot(q)) - t.y();
	});
}

std::string Gin::Module::Signed::SDTorus::GetName()
{
	return "SDTorus";
}*/
