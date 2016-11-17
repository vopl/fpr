#include "signal.hpp"

namespace tst
{
    Signal::Signal()
    {

    }

    Signal::~Signal()
    {

    }


    void Signal::setup(real frequency, std::size_t keepHistorySamples)
    {
        _frequency = frequency;
        _keepHistorySamples = keepHistorySamples;
    }


    void Signal::stepTo(real t)
    {
        assert(0);
    }


    void Signal::pushHarmonicSegment(
            real t0, real t1,
            real f0, real f1,
            real a0, real a1)
    {
        assert(0);
    }


}
