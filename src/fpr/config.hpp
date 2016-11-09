#pragma once

#include "math.hpp"

namespace aid { namespace fpr
{
    struct Config
    {
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        std::size_t _signalFrequency;
        std::size_t _signalBucketSize;


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        TVReal      _cqtFrequencyGrid;
        TVReal      _cqtPpwGrid;


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        std::size_t _trajectorizerFindMaxHeight;//periods


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        real _timeGridSmoothRange = 50;//ms
        real _timeGridMinStep = 100;//ms


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        std::size_t _valueGridSize = 32;//штук
        real _valueGridSmoothWidth = 50;//ms

        std::size_t _valuesBits = 1;

    };
}}
