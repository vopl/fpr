#include "harmonicGenerator.hpp"

namespace tst
{
    HarmonicGenerator::HarmonicGenerator()
    {

    }

    HarmonicGenerator::~HarmonicGenerator()
    {

    }


    void HarmonicGenerator::setup(
            HarmonicGeneratorRandomLaw frequencyLaw,
            HarmonicGeneratorRandomLaw amplitudeLaw)
    {
        _frequencyLaw = frequencyLaw;
        _amplitudeLaw = amplitudeLaw;

        Point initialPoint;

        {
            initialPoint._time = -1;

            initialPoint._value = real(rand())/RAND_MAX * (_frequencyLaw._max - _frequencyLaw._min) + _frequencyLaw._min;
            initialPoint._valueD1 = 0;
            _frequencyPoints.push_back(initialPoint);

            initialPoint._time = -0.5;
            _frequencyPoints.push_back(initialPoint);
        }
        while(generateNewPointIfNeed(_frequencyPoints, _frequencyLaw, 0, 0))
        {
            //empty ok
        }


        {
            initialPoint._time = -1;

            initialPoint._value = real(rand())/RAND_MAX * (_amplitudeLaw._max - _amplitudeLaw._min) + _amplitudeLaw._min;
            initialPoint._valueD1 = 0;
            _amplitudePoints.push_back(initialPoint);

            initialPoint._time = -0.5;
            _amplitudePoints.push_back(initialPoint);
        }
        while(generateNewPointIfNeed(_amplitudePoints, _amplitudeLaw, 0, 0))
        {
            //empty ok
        }

        _prevTime = 0;
        _time = 0;
    }


    void HarmonicGenerator::stepTo(real time)
    {
        _prevTime = _time;
        _time = time;

        while(generateNewPointIfNeed(_frequencyPoints, _frequencyLaw, _prevTime, _time))
        {
            //empty ok
        }
        while(generateNewPointIfNeed(_amplitudePoints, _amplitudeLaw, _prevTime, _time))
        {
            //empty ok
        }
    }


    void HarmonicGenerator::flush(Spectra &grid)
    {
        grid.pushHarmonicSegment(
                    _prevTime, _time,
                    f(_prevTime), f(_time),
                    a(_prevTime), a(_time));
    }

    void HarmonicGenerator::flush(Signal &signal)
    {
        assert(0);
    }


    bool HarmonicGenerator::generateNewPointIfNeed(std::deque<Point> &points, const HarmonicGeneratorRandomLaw &law, real minTime, real maxTime)
    {
        while(points[points.size()-2]._time < maxTime)
        {
            Point &point0 = points[points.size()-2];
            Point &point1 = points[points.size()-1];
            Point point2;

            point2._time = point1._time + law._timeChangeGenerator->generate();
            point2._value = point1._value + law._valueChangeGenerator->generate();

            if(point2._value > law._max)
            {
                point2._value = law._max;
            }

            if(point2._value < law._min)
            {
                point2._value = law._min;
            }

            point1._valueD1 = (point2._value - point1._value)/2/(point2._time - point1._time) +
                              (point1._value - point0._value)/2/(point1._time - point0._time);

            points.push_back(point2);
        }

        while(points[1]._time < minTime)
        {
            points.pop_front();
        }

    }

    real HarmonicGenerator::f(real time)
    {
        return ermit(_frequencyPoints, time);
    }

    real HarmonicGenerator::a(real time)
    {
        return ermit(_amplitudePoints, time);
    }

    real HarmonicGenerator::ermit(const std::deque<Point> &points, real time)
    {
        assert(points.size()>1);
        assert(points.front()._time < time);
        assert(points.back()._time > time);

        std::size_t i(points.size()-2);
        while(points[i]._time > time)
        {
            i--;
        }

        const Point &p0 = points[i];
        const Point &p1 = points[i+1];

        real dtime = p1._time - p0._time;

        real t = (time - p0._time) / dtime;
        real t2 = t*t;
        real t3 = t2*t;


        return
                + (+2*t3 - 3*t2 + 0*t + 1) * p0._value
                + (+1*t3 - 2*t2 + 1*t + 0) * (dtime)*p0._valueD1

                + (-2*t3 + 3*t2 + 0*t + 0) * p1._value
                + (+1*t3 - 1*t2 + 0*t + 0) * (dtime)*p1._valueD1;
    }

}
