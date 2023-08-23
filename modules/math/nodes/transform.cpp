#include "transform.hpp"

#include <Eigen/Dense>

Gin::Module::Math::Translate::Translate()
{
	AddInputPort("In", in);
	AddInputPort("Translate", translate);

	AddOutputPort("Out", out);
}

void Gin::Module::Math::Translate::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		out[idx] = in[idx] + translate[idx];
	});
}

std::string Gin::Module::Math::Translate::GetName()
{
	return "Translate";
}

Gin::Module::Math::Rotate::Rotate()
{
	AddInputPort("In", in);
	AddInputPort("Rotate", rotate);

	AddOutputPort("Out", out);
}

void Gin::Module::Math::Rotate::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		double cr = std::cos(rotate[idx].x() * 0.5);
		double sr = std::sin(rotate[idx].x() * 0.5);
		double cp = std::cos(rotate[idx].y() * 0.5);
		double sp = std::sin(rotate[idx].y() * 0.5);
		double cy = std::cos(rotate[idx].z() * 0.5);
		double sy = std::sin(rotate[idx].z() * 0.5);
		Eigen::Quaternion<double> q{
			cr * cp * cy + sr * sp * sy,
			sr * cp * cy - cr * sp * sy,
			cr * sp * cy + sr * cp * sy,
			cr * cp * sy - sr * sp * cy
		};

		out[idx] = q * in[idx];
	});
}

std::string Gin::Module::Math::Rotate::GetName()
{
	return "Rotate";
}

Gin::Module::Math::Scale::Scale()
{
	AddInputPort("In", in);
	AddInputPort("Scale", scale);

	AddOutputPort("Out", out);
}

void Gin::Module::Math::Scale::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		out[idx] = in[idx].cwiseProduct(scale[idx]);
	});
}

std::string Gin::Module::Math::Scale::GetName()
{
	return "Scale";
}

Gin::Module::Math::DomainRepeat::DomainRepeat()
{
	AddInputPort("In", in);
	AddInputPort("Domain Extent", domain);

	AddOutputPort("Out", out);
	AddOutputPort("Id", domainPosition);
}

void Gin::Module::Math::DomainRepeat::Execute(Graph::GraphContext ctx)
{
	SpatialOperation([&](size_t idx, size_t _x, size_t _y, size_t _z) {
		domainPosition[idx].x() = std::floor((in[idx].x() + domain.x()) / (domain.x() * 2));
		domainPosition[idx].y() = std::floor((in[idx].y() + domain.y()) / (domain.y() * 2));
		domainPosition[idx].z() = std::floor((in[idx].z() + domain.z()) / (domain.z() * 2));

		out[idx] = in[idx] - domainPosition[idx].cwiseProduct(domain * 2);
	});
}

std::string Gin::Module::Math::DomainRepeat::GetName()
{
	return "Domain Repeat";
}
