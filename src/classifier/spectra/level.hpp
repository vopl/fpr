#pragma once
#include "common.hpp"

namespace tst { namespace spectra
{
    class Level
    {
    public:
        Level(real frequency,
              real frequencyHeight,
              real periodsBefore, real periodsInTarget, real periodsAfter,
              std::size_t samplesPerPeriod);
        ~Level();

        void stepTo(real time);

        void pushHarmonicSegment(real t0, real t1,
                real f0, real f1,
                real a0, real a1);

        std::size_t dataSize();
        real &data(std::size_t index);

    private:
        real &target(std::size_t pos);

    private:
        real        _frequency;
        real        _frequencyHeight;

        real        _periodsBefore;
        real        _periodsInTarget;
        real        _periodsAfter;

        std::size_t _samplesPerPeriod;

    private:
        real _time;
        std::vector<real> _targetBuffer;
        std::size_t _targetBufferSize;
        std::size_t _targetBufferEnd;

    };

    using LevelPtr = std::unique_ptr<Level>;

}}
