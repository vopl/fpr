#include "fpr/timeGrid.hpp"
#include <iostream>
#include <fstream>

namespace fpr
{
    TimeGrid::TimeGrid(const Config *config)
        : _config(config)
    {
        std::size_t timeGridSmoothRange = std::size_t(real(_config->_timeGridSmoothRange) * _config->_signalFrequency / _config->_signalBucketSize / 1000 + 0.5);
        _echoSum.resize(timeGridSmoothRange);

        std::size_t timeGridMinStep = std::size_t(real(_config->_timeGridMinStep) * _config->_signalFrequency / _config->_signalBucketSize / 1000 + 0.5);
        _echoSmoothed.resize(timeGridMinStep);
    }

    TimeGrid::~TimeGrid()
    {

    }

    void TimeGrid::enplaceEcho(const real *echo, bool &hasPeculiar)
    {
        hasPeculiar = false;

        {
            real mean = 0;
            for(std::size_t i(0); i<_config->_cqtFrequencyGrid.size(); ++i)
            {
                mean += echo[i];
            }

            //mean /= _config->_frequencyGrid.size();

            real sum = _echoSum.back() + mean;
            _echoSum.push_back(sum);
        }

        real smoothed = (_echoSum.back() - _echoSum.front()); // /real(_echoSumSerie.size()-1);

        _echoSmoothed.push_back(smoothed);

        if(smoothed > maxValue)
        {
            maxValue = smoothed;
            maxPos = _echoSmoothed.size();
        }

        if(maxPos > 0)
        {
            maxPos--;

            if(!maxPos)
            {
                hasPeculiar = true;
                _lastPeculiarPosition = _echosPushed-_echoSmoothed.size()-1-_echoSum.size()/2;
            }
        }
        else
        {
            maxValue = _echoSmoothed.front();
        }

        _echosPushed++;

        {
            static const std::size_t timeGridFlushSumSteps = 1000;//signal buckets

            if(!(_echosPushed % timeGridFlushSumSteps))
            {
                real sum = _echoSum.back();
                //std::cout<<"flush err: "<<sum<<std::endl;

                auto a1 = _echoSum.array_one();
                for(std::size_t i(0); i<a1.second; ++i)
                {
                    a1.first[i] -= sum;
                }
                auto a2 = _echoSum.array_two();
                for(std::size_t i(0); i<a2.second; ++i)
                {
                    a2.first[i] -= sum;
                }
            }
        }
    }

    std::size_t TimeGrid::lastPeculiarPosition()
    {
        return _lastPeculiarPosition;
    }

}
