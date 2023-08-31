#include "primitives.hpp"

Gin::Module::Signed::SDSphere::SDSphere()
{
	AddInputPort("Radius", radius);
	AddInputPort("Position", position);

	AddOutputPort("Signed Distance", distance);
}

void Gin::Module::Signed::SDSphere::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t x, size_t y, size_t z) {
		distance[idx] = sqrt(position[idx].dot(position[idx])) - radius[idx];
	});
}

void Gin::Module::Signed::SDSphere::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t x, size_t y, size_t z) {
		distance[idx] = sqrt(position[idx].dot(position[idx])) - radius[idx];
	});
}

std::string Gin::Module::Signed::SDSphere::GetName()
{
	return "SDSphere";
}

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
}
