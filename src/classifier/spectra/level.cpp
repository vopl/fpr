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
        _targetBuffer.resize(std::size_t(_samplesPerPeriod * _periodsInTarget + 0.5));
        _targetBufferEnd = 0;
    }

    Level::~Level()
    {

    }

    void Level::stepTo(real time)
    {
        real newTargetBufferEndTime = time - _periodsBefore/_frequency;
        if(newTargetBufferEndTime <= 0)
        {
            return;
        }

        std::size_t newTargetBufferEnd = std::size_t(newTargetBufferEndTime*_frequency*_samplesPerPeriod);
        assert(newTargetBufferEnd >= _targetBufferEnd);

//        std::size_t dist = newTargetBufferEnd - _targetBufferEnd;
//        std::cout<<_frequency<<" -> "<<dist<<std::endl;

        //rotate target
        while(newTargetBufferEnd > _targetBufferEnd)
        {
            target(_targetBufferEnd) = real();
            _targetBufferEnd++;
        }

        _time = time;
    }

    void Level::pushHarmonicSegment(
            real t0, real t1,
            real f0, real f1,
            real a0, real a1)
    {
        //нормировка времени и частоты на единицы данного уровня
        real tNorma = _samplesPerPeriod/_frequency;
        real fNorma = _frequencyHeight;

        //время отступить на начало целевого сегмента
        t0 -= (_periodsBefore+_periodsInTarget)/_frequency;
        t1 -= (_periodsBefore+_periodsInTarget)/_frequency;

        t0 /= tNorma;
        t1 /= tNorma;

        f0 -= _frequency;
        f1 -= _frequency;

        f0 /= fNorma;
        f1 /= fNorma;

        std::size_t idx0 = std::size_t(std::max(real(0), t0));
        std::size_t idx1 = std::size_t(std::max(real(0), t1)) + 1;

        for(std::size_t idx(idx0); idx<idx1; ++idx)
        {
            real tCenter = real(idx)+real(0.5);
            real fCenter = real(0.5);

            real tl0 = t0 - tCenter;
            real tl1 = t1 - tCenter;

            real fl0 = f0 - fCenter;
            real fl1 = f1 - fCenter;

            real al0 = a0;
            real al1 = a1;

            //прямая A * t + B * f + C = 0
            real fA = +real(1.0)/(tl1-tl0);
            real fB = -real(1.0)/(fl1-fl0);
            real fC = -(tl0)/(tl1-tl0) + (fl0)/(fl1-fl0);

            //пересечь прямую сcomplex единичной окружностью
            real a = fB*fB + fA*fA;
            real b = 2*fA*fC;
            real c = fC*fC - 1.0;

            real d = b*b - 4*a*c;

            if(d <= 0)
            {
                //нет пересечений
                continue;
            }

            //точки пересечения с окружностью
            real tc0 = (-b - sqrt(d))/2/a;
            real tc1 = (-b + sqrt(d))/2/a;

            real fc0 = - (fA*tc0 + fC) / fB;
            real fc1 = - (fA*tc1 + fC) / fB;

            //прямая A * t + B * a + C = 0
            real aA = +real(1.0)/(tl1-tl0);
            real aB = -real(1.0)/(al1-al0);
            real aC = -(tl0)/(tl1-tl0) + (al0)/(al1-al0);

            //теперь ограничить оригинальный отрезок
            if(tl0 < tc0)
            {
                fl0 = fc0;
                al0 = - (aA*tc0 + aC) / aB;
                tl0 = tc0;
            }

            if(tl1 > tc1)
            {
                fl1 = fc1;
                al1 = - (aA*tc1 + aC) / aB;
                tl1 = tc1;
            }

            if(tl0 >= tl1)
            {
                //отрезок получился наизнанку
                continue;
            }

            //интеграл амплитуды по времени, от tl0 до tl1
            real ai = (-aA/aB*tl1*tl1/2 + tl1) - (-aA/aB*tl0*tl0/2 + tl0);

            //и поделить его на диаметр (если прямая проходит ровно через центр) - это и есть вклад
            target(_targetBufferEnd-_targetBuffer.size() + idx) += ai / 2;
        }

    }

    real &Level::target(std::size_t pos)
    {
        return _targetBuffer[pos % _targetBuffer.size()];
    }


}}
