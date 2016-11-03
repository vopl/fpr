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


        std::vector<std::vector<real>> _trajectorizerSmoothFilter;//signal buckets x periods
        std::size_t _trajectorizerFindMaxHeight;//periods

        std::size_t _trajectorizerLineSmoothWidth;//signal buckets
    };
}
