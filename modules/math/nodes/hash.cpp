#include "hash.hpp"


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
}
