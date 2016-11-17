#pragma once
#include "common.hpp"

#include "randomGenerator.hpp"
#include "spectra.hpp"
#include "signal.hpp"

namespace tst
{
    struct HarmonicGeneratorRandomLaw
    {
        real _min = 0;
        real _max = 1;

        RandomGenerator *_timeChangeGenerator = nullptr;//генератор изменения времени
        RandomGenerator *_valueChangeGenerator = nullptr;//генератор изменения значения
    };

    class HarmonicGenerator
    {
    public:
        HarmonicGenerator();
        ~HarmonicGenerator();

        void setup(
                HarmonicGeneratorRandomLaw frequencyLaw,
                HarmonicGeneratorRandomLaw amplitudeLaw);

        void stepTo(real time);

        void flush(Spectra &grid);
        void flush(Signal &signal);

    private:
        struct Point;
        bool generateNewPointIfNeed(std::deque<Point> &points, const HarmonicGeneratorRandomLaw &law, real minTime, real maxTime);

        real f(real time);
        real a(real time);
        real ermit(const std::deque<Point> &points, real time);

    private:
        HarmonicGeneratorRandomLaw _frequencyLaw;
        HarmonicGeneratorRandomLaw _amplitudeLaw;

    private:
        real _prevTime;
        real _time;

        struct Point
        {
            real _time = 0;

            real _value = 0;
            real _valueD1 = 0;
        };

        //для эрмитова сплайна, как минимум 4 точки, текущее время между точками 1 и N-2
        std::deque<Point> _frequencyPoints;
        std::deque<Point> _amplitudePoints;
    };

}
