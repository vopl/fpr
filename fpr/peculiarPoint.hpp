#pragma once

#include "fpr/math.hpp"

namespace fpr
{
    struct PeculiarPoint
    {
        enum class Type
        {
            minY,
            maxY,

            maxV,
        };

        Type _type;

        std::size_t _x;
        std::size_t _y;

        real _v;
    };
}
