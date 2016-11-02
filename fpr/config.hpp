#pragma once

#include "fpr/math.hpp"

namespace fpr
{
    struct Config
    {
        std::size_t _signalFrequency;
        std::size_t _signalBucketSize;

        TVReal      _frequencyGrid;
        TVReal      _ppwGrid;
    };
}
