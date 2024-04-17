//
// Created by HITO on 17/04/24.
//

#ifndef GIN_PRIMITIVE_HPP
#define GIN_PRIMITIVE_HPP

#include <gin/field/field.hpp>
#include <gin/field/sampler.hpp>
#include <gin/math/type.hpp>

namespace Gin::SDF {

    class Primitive {
    public:
        virtual Field::Sampler<float> Compute(Field::Sampler<Math::Vector3> position) = 0;
    };

}

#endif //GIN_PRIMITIVE_HPP
