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

        _samplesAmount = _keepHistorySamples;
        _samples.resize(_samplesAmount);
        std::fill(_samples.begin(), _samples.end(), real(0));
        _samplesEnd = 0;

        _time = 0;
    }


    void Signal::stepTo(real time)
    {
        std::size_t newSamplesEnd = std::size_t(time*_frequency+1.0);
        assert(newSamplesEnd >= _samplesEnd);

        //rotate target
        while(newSamplesEnd > _samplesEnd)
        {
            sample(_samplesEnd) = 0;
            _samplesEnd++;
        }

        _time = time;
    }


    void Signal::pushHarmonicSegment(
            real t0, real t1,
            real f0, real f1,
            real a0, real a1)
    {
        //прямая f = fK * t + fB
        real fK = (f1-f0) / (t1-t0);
        real fB = f0 - fK*t0;

        //прямая a = aK * t + aB
        real aK = (a1-a0) / (t1-t0);
        real aB = a0 - aK*t0;


        std::size_t beginIdx = std::size_t(t0*_frequency);
        std::size_t endIdx = std::size_t(t1*_frequency);

        {
            std::size_t sampleIdx(beginIdx);

            real t = real(sampleIdx) / _frequency;
            real a = aK*t + aB;
            real f = fK*t + fB;

            real part = 1.0 - (t-t0) * _frequency;

            sample(sampleIdx) += a * cos(M_PI*2*t*f) * part;
        }

        for(std::size_t sampleIdx(beginIdx+1); sampleIdx<endIdx; ++sampleIdx)
        {
            real t = real(sampleIdx) / _frequency;
            real a = aK*t + aB;
            real f = fK*t + fB;
            sample(sampleIdx) += a * cos(M_PI*2*t*f);
        }

        {
            std::size_t sampleIdx(endIdx);

            real t = real(sampleIdx) / _frequency;
            real a = aK*t + aB;
            real f = fK*t + fB;

            real part = (t1-t) * _frequency;

            sample(sampleIdx) += a * cos(M_PI*2*t*f) * part;
        }
    }

    std::size_t Signal::dataSize()
    {
        return _samplesAmount;
    }

    real &Signal::data(std::size_t index)
    {
        return sample(_samplesEnd + index);
    }

    real &Signal::sample(std::size_t pos)
    {
        return _samples[pos % _samplesAmount];
    }

}
