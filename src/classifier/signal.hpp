#pragma once
#include "common.hpp"

namespace tst
{
    class Signal
    {
    public:
        Signal();
        ~Signal();

        void setup(real frequency, std::size_t keepHistorySamples);

        void stepTo(real t);

        void pushHarmonicSegment(
                real t0, real t1,
                real f0, real f1,
                real a0, real a1);

    private:
        real _frequency = 8000;
        std::size_t _keepHistorySamples = 8000;
    };

}
