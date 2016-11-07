#include "fpr/valueGrid.hpp"
#include <algorithm>
#include <iostream>

namespace fpr
{
    ValueGrid::ValueGrid(const Config *config)
        : _config(config)
    {
        _smoothWidth = std::max(
                           std::size_t(1),
                           std::size_t(real(_config->_valueGridSmoothWidth) * _config->_signalFrequency / _config->_signalBucketSize / 1000 + 0.5));

        std::size_t echoHistorySize = std::size_t(real(_config->_timeGridMinStep*2) * _config->_signalFrequency / _config->_signalBucketSize / 1000 + 0.5) + _smoothWidth;
        _echo.resize(echoHistorySize);

        for(auto &v : _echo)
        {
            v.reset(new real[_config->_cqtFrequencyGrid.size()]);
        }
    }

    ValueGrid::~ValueGrid()
    {

    }

    void ValueGrid::enplaceEcho(const real *echo)
    {
        real *own = getEcho(_echosPushed);


        //find maxes
        std::fill(own, own+_config->_cqtFrequencyGrid.size(), real(0));

        {
            std::size_t h = _config->_trajectorizerFindMaxHeight;
            std::size_t hDown = h/2;
            std::size_t hUp = h - hDown;

            for(std::size_t y(hDown); y<_config->_cqtFrequencyGrid.size()-hUp; ++y)
            {
                real v = echo[y];
                bool isMax = true;
                for(std::size_t y1(y-hDown); y1<y+hUp; ++y1)
                {
                    if(y1!=y && echo[y1] >= v)
                    {
                        isMax = false;
                        break;
                    }
                }

                if(isMax)
                {
                    own[y] = echo[y];
                }
            }
        }

        _echosPushed++;
    }

    void ValueGrid::flush(std::size_t pos, real *values)
    {
        real *valuesEnd = values + _config->_valueGridSize;

        std::fill(values, valuesEnd, real(0));


        assert(_echosPushed - _echo.size() < pos);


        std::size_t h = _config->_trajectorizerFindMaxHeight;
        std::size_t hDown = h/2;
        std::size_t hUp = h - hDown;

        for(std::size_t x(pos - _smoothWidth/2); x<pos+_smoothWidth-_smoothWidth/2; ++x)
        {
            assert(x < _echosPushed);

            const real *echo = getEcho(x);

            for(std::size_t y(hDown); y<_config->_cqtFrequencyGrid.size()-hUp; ++y)
            {
                std::size_t ry = (y-hDown)*_config->_valueGridSize/(_config->_cqtFrequencyGrid.size() - h);

                assert(ry < _config->_valueGridSize);

                values[ry] += echo[y];
            }
        }

        real min = std::numeric_limits<real>::max();
        real max = -std::numeric_limits<real>::max();
        for(std::size_t ry(1); ry<_config->_valueGridSize-1; ++ry)
        {
            values[ry] = std::pow(values[ry], real(0.25));

            if(values[ry] < min)
            {
                min = values[ry];
            }
            if(values[ry] > max)
            {
                max = values[ry];
            }
        }

        if(max > min)
        {
            for(std::size_t ry(0); ry<_config->_valueGridSize; ++ry)
            {
                values[ry] = std::max(real(0), std::min(real(1), (values[ry]-min)/(max-min)));
            }
        }
    }

    real *ValueGrid::getEcho(std::size_t pos)
    {
        return _echo[pos % _echo.size()].get();
    }

}
