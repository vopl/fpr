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

        void stepTo(real time);

        void pushHarmonicSegment(
                real t0, real t1,
                real f0, real f1,
                real a0, real a1);

        std::size_t dataSize();
        real &data(std::size_t index);

    private:
        real &sample(std::size_t pos);

    private:
        real _frequency = 8000;
        std::size_t _keepHistorySamples = 8000;


        real _time = 0;
        std::vector<real> _samples;
        std::size_t _samplesAmount;
        std::size_t _samplesEnd;

    };

}
