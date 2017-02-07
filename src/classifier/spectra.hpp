#pragma once
#include "common.hpp"
#include "spectra/level.hpp"
#include <vector>

namespace tst
{
    class Spectra
    {
    public:
        Spectra();
        ~Spectra();

        void setup(
                real fMin, real fMax,
                std::size_t fSteps,

                real periodsBefore, real periodsInTarget, real periodsAfter,
                std::size_t samplesPerPeriod);

        void stepTo(real time);

        void pushHarmonicSegment(
                real t0, real t1,
                real f0, real f1,
                real a0, real a1);

        real &data(std::size_t levelIdx, std::size_t sampleIdx);

        void draw(const char *fname);// periodsInTarget*samplesPerPeriod x fSteps

    private:
        real        _fMin = 20;
        real        _fMax = 20000;
        std::size_t _fSteps = 1000;
        real        _fStep = (_fMax - _fMin)/_fSteps;

        real        _periodsBefore = 10;
        real        _periodsInTarget = 10;
        real        _periodsAfter = 10;

        std::size_t _samplesPerPeriod = 12;

    private:
        std::vector<spectra::LevelPtr> _levels;
    };

}
