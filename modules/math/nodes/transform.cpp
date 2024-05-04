#include "transform.hpp"

#include <xsimd/xsimd.hpp>

Gin::Module::Math::DomainRepeat::DomainRepeat() {
    AddInputPort("In", in);
    AddInputPort("Domain Size", domainSize);

    AddOutputPort("Out", out);
    AddOutputPort("Domain Position", domainPosition);
}

void Gin::Module::Math::DomainRepeat::Initialize(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    out.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
    domainPosition.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
}

void Gin::Module::Math::DomainRepeat::Execute(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::VectorizedVector3Field> o = out.GetField<Field::VectorizedVector3Field>();
    std::shared_ptr<Field::VectorizedVector3Field> p = domainPosition.GetField<Field::VectorizedVector3Field>();

    constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSizeW) {
                auto p1 = in.GetVector3Batch(x, y, z);
                auto s = domainSize.GetVector3Batch(x, y, z);

                p1.c[0] = xsimd::floor((p1.c[0] + s.c[0]) / (s.c[0] * 2));
                p1.c[1] = xsimd::floor((p1.c[1] + s.c[1]) / (s.c[1] * 2));
                p1.c[2] = xsimd::floor((p1.c[2] + s.c[2]) / (s.c[2] * 2));

                Field::VectorizedVector3Field::VectorVector3& vv3p = p->GetVectorVector3(idx);
                xsimd::store_aligned(vv3p.x, p1.c[0]);
                xsimd::store_aligned(vv3p.y, p1.c[1]);
                xsimd::store_aligned(vv3p.z, p1.c[2]);

                auto p2 = in.GetVector3Batch(x, y, z);

                p2.c[0] = p2.c[0] - p1.c[0] * (s.c[0] * 2);
                p2.c[1] = p2.c[1] - p1.c[1] * (s.c[1] * 2);
                p2.c[2] = p2.c[2] - p1.c[2] * (s.c[2] * 2);

                Field::VectorizedVector3Field::VectorVector3& vv3o = o->GetVectorVector3(idx);
                xsimd::store_aligned(vv3o.x, p2.c[0]);
                xsimd::store_aligned(vv3o.y, p2.c[1]);
                xsimd::store_aligned(vv3o.z, p2.c[2]);

                ++idx;
            }
        }
    }
}

std::string Gin::Module::Math::DomainRepeat::GetName() {
    return "Domain Repeat";
}

Gin::Module::Math::Translate::Translate() {
    AddInputPort("In", in);
    AddInputPort("Translation", translation);

    AddOutputPort("Out", out);
}

void Gin::Module::Math::Translate::Initialize(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    out.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
}

void Gin::Module::Math::Translate::Execute(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::VectorizedVector3Field> o = out.GetField<Field::VectorizedVector3Field>();

    constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSizeW) {
                auto p1 = in.GetVector3Batch(x, y, z);
                auto t = translation.GetVector3Batch(x, y, z);

                p1.c[0] = p1.c[0] + t.c[0];
                p1.c[1] = p1.c[1] + t.c[1];
                p1.c[2] = p1.c[2] + t.c[2];

                Field::VectorizedVector3Field::VectorVector3& vv3 = o->GetVectorVector3(idx);
                xsimd::store_aligned(vv3.x, p1.c[0]);
                xsimd::store_aligned(vv3.y, p1.c[1]);
                xsimd::store_aligned(vv3.z, p1.c[2]);

                ++idx;
            }
        }
    }
}

std::string Gin::Module::Math::Translate::GetName() {
    return "Translate";
}

Gin::Module::Math::Rotate::Rotate() {
    AddInputPort("In", in);
    AddInputPort("Rotation", rotation);

    AddOutputPort("Out", out);
}

void Gin::Module::Math::Rotate::Initialize(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    out.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
}

void Gin::Module::Math::Rotate::Execute(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::VectorizedVector3Field> o = out.GetField<Field::VectorizedVector3Field>();

    constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSizeW) {
                auto p1 = in.GetVector3Batch(x, y, z);
                auto r = rotation.GetVector3Batch(x, y, z);

                r.c[0] *= 0.0174533;
                r.c[1] *= 0.0174533;
                r.c[2] *= 0.0174533;

                auto cr = xsimd::cos(r.c[0] * 0.5);
                auto sr = xsimd::sin(r.c[0] * 0.5);
                auto cp = xsimd::cos(r.c[1] * 0.5);
                auto sp = xsimd::sin(r.c[1] * 0.5);
                auto cy = xsimd::cos(r.c[2] * 0.5);
                auto sy = xsimd::sin(r.c[2] * 0.5);

                auto s = cr * cp * cy + sr * sp * sy;
                r.c[0] = sr * cp * cy - cr * sp * sy;
                r.c[1] = cr * sp * cy + sr * cp * sy;
                r.c[2] = cr * cp * sy - sr * sp * cy;

                cr = r.c[1] * p1.c[2] - r.c[2] * p1.c[1];
                sr = -(r.c[0] * p1.c[2] - r.c[2] * p1.c[0]);
                cp = r.c[0] * p1.c[1] - r.c[1] * p1.c[0];

                sp = (r.c[1] * cp - r.c[2] * sr) * 2.0;
                cy = (-(r.c[0] * cp - r.c[2] * cr)) * 2.0;
                sy = (r.c[0] * sr - r.c[1] * cr) * 2.0;

                cr = 2.0 * s * cr;
                sr = 2.0 * s * sr;
                cp = 2.0 * s * cp;

                r.c[0] = p1.c[0] + cr + sp;
                r.c[1] = p1.c[1] + sr + cy;
                r.c[2] = p1.c[2] + cp + sy;

                Field::VectorizedVector3Field::VectorVector3& vv3 = o->GetVectorVector3(idx);
                xsimd::store_aligned(vv3.x, r.c[0]);
                xsimd::store_aligned(vv3.y, r.c[1]);
                xsimd::store_aligned(vv3.z, r.c[2]);

                ++idx;
            }
        }
    }
}

std::string Gin::Module::Math::Rotate::GetName() {
    return "Rotate";
}


Gin::Module::Math::Scale::Scale() {
    AddInputPort("In", in);
    AddInputPort("Scale", scale);

    AddOutputPort("Out", out);
}

void Gin::Module::Math::Scale::Initialize(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    out.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
}

void Gin::Module::Math::Scale::Execute(Graph::GraphContext ctx) {
    Gin::Math::Vector3 size{ Gin::Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

    std::shared_ptr<Field::VectorizedVector3Field> o = out.GetField<Field::VectorizedVector3Field>();

    constexpr size_t simdSizeW = xsimd::simd_type<Gin::Math::Scalar>::size;
    size_t idx = 0;

    for (size_t z = 0; z < size.z; ++z) {
        for (size_t y = 0; y < size.y; ++y) {
            for (size_t x = 0; x < size.x; x += simdSizeW) {
                auto p1 = in.GetVector3Batch(x, y, z);
                auto s = scale.GetVector3Batch(x, y, z);

                p1.c[0] = p1.c[0] / s.c[0];
                p1.c[1] = p1.c[1] / s.c[1];
                p1.c[2] = p1.c[2] / s.c[2];

                Field::VectorizedVector3Field::VectorVector3& vv3 = o->GetVectorVector3(idx);
                xsimd::store_aligned(vv3.x, p1.c[0]);
                xsimd::store_aligned(vv3.y, p1.c[1]);
                xsimd::store_aligned(vv3.z, p1.c[2]);

                ++idx;
            }
        }
    }
}

std::string Gin::Module::Math::Scale::GetName() {
    return "Scale";
}

/*
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

void Gin::Module::Math::Translate::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
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

void Gin::Module::Math::Rotate::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
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

void Gin::Module::Math::Scale::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
		out[idx] = in[idx].cwiseProduct(scale[idx]);
	});
}

std::string Gin::Module::Math::Scale::GetName()
{
	return "Scale";
}

Gin::Module::Math::Transform::Transform()
{
	AddInputPort("In", in);
	AddInputPort("Translate", translate);
	AddInputPort("Scale", rotate);
	AddInputPort("Scale", scale);

	AddOutputPort("Out", out);
}

void Gin::Module::Math::Transform::Execute(Graph::GraphContext ctx)
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

		out[idx] = translate[idx] + (q * in[idx].cwiseProduct(scale[idx]));
	});
}

void Gin::Module::Math::Transform::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
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

		out[idx] = translate[idx] + (q * in[idx].cwiseProduct(scale[idx]));
	});
}

std::string Gin::Module::Math::Transform::GetName()
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

void Gin::Module::Math::DomainRepeat::Execute(Graph::GraphContext ctx, Thread::ThreadPool& pool)
{
	SpatialOperation(pool, [&](size_t idx, size_t _x, size_t _y, size_t _z) {
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
*/