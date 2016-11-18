#include "level.hpp"


namespace tst { namespace spectra
{
    Level::Level(real frequency,
                 real frequencyHeight,
                 real periodsBefore, real periodsInTarget, real periodsAfter,
                 std::size_t samplesPerPeriod)
        : _frequency(frequency)
        , _frequencyHeight(frequencyHeight)
        , _periodsBefore(periodsBefore)
        , _periodsInTarget(periodsInTarget)
        , _periodsAfter(periodsAfter)
        , _samplesPerPeriod(samplesPerPeriod)
        , _time(0)
    {
        _targetBufferSize = std::size_t(_samplesPerPeriod * (_periodsInTarget + _periodsBefore) + 0.5);
        _targetBuffer.resize(_targetBufferSize);
        _targetBufferEnd = 0;
    }

    Level::~Level()
    {

    }

    void Level::stepTo(real time)
    {
        std::size_t newTargetBufferEnd = std::size_t(time*_frequency*_samplesPerPeriod+1.0);
        assert(newTargetBufferEnd >= _targetBufferEnd);

//        std::size_t dist = newTargetBufferEnd - _targetBufferEnd;
//        std::cout<<_frequency<<" -> "<<dist<<std::endl;

        //rotate target
        while(newTargetBufferEnd > _targetBufferEnd)
        {
            target(_targetBufferEnd) = 0;//(_targetBufferEnd % _samplesPerPeriod);
            _targetBufferEnd++;
        }

        _time = time;
    }

    void Level::pushHarmonicSegment(
            real t0, real t1,
            real f0, real f1,
            real a0, real a1)
    {
        //время отступить на начало целевого сегмента
        real targetBufferStartTime = (real(_targetBufferEnd+0)-_targetBufferSize)/_samplesPerPeriod/_frequency;
        t0 -= targetBufferStartTime;
        t1 -= targetBufferStartTime;

        //частоту отступить на начало собственный центр
        f0 -= _frequency;
        f1 -= _frequency;

        //нормировка времени и частоты на единицы данного уровня
        real tNorma = 1.0/_frequency/_samplesPerPeriod;
        real fNorma = _frequencyHeight;

        t0 /= tNorma;
        t1 /= tNorma;

        f0 /= fNorma;
        f1 /= fNorma;


        //прямая f = fK * t + fB
        real fK = (f1-f0) / (t1-t0);
        real fB = f0 - fK*t0;

        //найти пересечение частоты с нулем, пригодится
        real tfZero = -std::numeric_limits<real>::max();
        real tfM1 = -std::numeric_limits<real>::max();
        real tfP1 = -std::numeric_limits<real>::max();
        if(fK>0)
        {
            tfZero = (real(0)-fB)/fK;
            tfM1 = (real(-1)-fB)/fK;
            tfP1 = (real(1)-fB)/fK;
        }


        //прямая a = aK * t + aB
        real aK = (a1-a0) / (t1-t0);
        real aB = a0 - aK*t0;

        std::size_t idx0 = std::size_t(std::max(real(0), t0));
        std::size_t idx1 = std::size_t(std::min(real(_targetBufferSize), t1+1));

        for(std::size_t idx(idx0); idx<idx1; ++idx)
        {
            //локального отрезка - время
            real lt0 = std::max(t0, real(idx));
            real lt1 = std::min(t1, real(idx+1));

            auto integrate = [&](real lt0, real lt1) -> real{

                real fMid = fK*(lt0+lt1)/2 + fB;

                if(fMid < -1.0 || fMid > 1.0)
                {
                    return 0;
                }

                auto itrMinus = [&](real t){
                    return
                            +(-aK*fK) * t*t*t/3
                            +(aK-aK*fB-fK*aB) * t*t/2
                            +(aB-fB*aB) * t;
                };

                auto itrPlus = [&](real t){
                    return
                            +(+aK*fK) * t*t*t/3
                            +(aK+aK*fB+fK*aB) * t*t/2
                            +(aB+fB*aB) * t;
                };

                if(fMid < 0)
                {
                    real res = itrPlus(lt1) - itrPlus(lt0);
                    return res;
                }

                real res = itrMinus(lt1) - itrMinus(lt0);
                return res;
            };

            real times[5] = {tfM1,tfZero,tfP1, lt0, lt1};
            std::sort(times, times+5);

            real ai = 0;
            for(std::size_t i(0); i<4; ++i)
            {
                if(times[i] >= lt0 && times[i+1]<=lt1)
                {
                    ai += integrate(times[i], times[i+1]);
                }
            }

            data(idx) += ai;
        }

    }

    std::size_t Level::dataSize()
    {
        return std::size_t(_samplesPerPeriod * (_periodsInTarget) + 0.5);
        //return _targetBufferSize;
    }

    real &Level::data(std::size_t index)
    {
        return target(_targetBufferEnd + index);
    }

    real &Level::target(std::size_t pos)
    {
        return _targetBuffer[pos % _targetBufferSize];
    }


}}
