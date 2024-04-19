#include "position.hpp"

#include <gin/math/math.hpp>
#include <xsimd/xsimd.hpp>

Gin::Module::Base::Position::Position()
{
	AddOutputPort("Position", position);
}

void Gin::Module::Base::Position::Initialize(Graph::GraphContext ctx) {
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };
    position.SetField(std::make_shared<Field::VectorizedVector3Field>(size.x, size.y, size.z));
}

void Gin::Module::Base::Position::Execute(Graph::GraphContext ctx)
{
    Math::Vector3 size{ Math::Ceil(ctx.bounds.extent * 2.0 / ctx.scale) };

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

                xc = xc * ctx.scale - ctx.bounds.extent.x + ctx.bounds.origin.x;
                yc = yc * ctx.scale - ctx.bounds.extent.y + ctx.bounds.origin.y;
                zc = zc * ctx.scale - ctx.bounds.extent.z + ctx.bounds.origin.z;

                Field::VectorizedVector3Field::VectorVector3& vv3 = f->GetVectorVector3(idx);
                xsimd::store_aligned(vv3.x, xc);
                xsimd::store_aligned(vv3.y, yc);
                xsimd::store_aligned(vv3.z, zc);

                ++idx;
            }
        }
    }
}

std::string Gin::Module::Base::Position::GetName()
{
	return "Position";
}
