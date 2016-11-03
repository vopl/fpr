#include "fpr/trajectorizer/line.hpp"
#include <cassert>

namespace fpr { namespace trajectorizer
{
    Line::Line(const Config *config, std::size_t x, std::vector<PeculiarPoint> &peculiars)
        : _config(config)
        , _xStart(x)
        , _peculiars(peculiars)
    {
    }

    Line::~Line()
    {
    }

    void Line::step(std::size_t y, real v)
    {
        real smoothedY;
        if(_smoothedYSum.empty())
        {
            _smoothedYSum.push_back(y);
            smoothedY = y;
        }
        else
        {
            _smoothedYSum.push_back(_smoothedYSum.back() + y);

            while(_smoothedYSum.size() > _config->_trajectorizerLineSmoothWidth)
            {
                _smoothedYSum.pop_front();
            }

            smoothedY = (_smoothedYSum.back() - _smoothedYSum.front())/(_smoothedYSum.size()-1);
        }

        _points.emplace_back(Point{_xStart + _points.size(), y, smoothedY, v});


        std::size_t w = _config->_trajectorizerLineSmoothWidth;


        if(_points.size() >= w)
        {
            std::size_t middleI = _points.size()-w/2;
            const Point &mp = _points[middleI];

            bool maxY = true;
            bool minY = true;
            bool maxV = true;

            for(std::size_t i(_points.size()-w); i<_points.size() && (maxY||minY||maxV); ++i)
            {
                if(middleI == i)
                {
                    continue;
                }

                const Point &p = _points[i];

                if(mp._smoothedY < p._smoothedY)
                {
                    maxY = false;
                }

                if(mp._smoothedY > p._smoothedY)
                {
                    minY = false;
                }

                if(mp._v < p._v)
                {
                    maxV = false;
                }
            }



            if(minY)
            {
                _peculiars.emplace_back(PeculiarPoint{
                                            PeculiarPoint::Type::minY,
                                            mp._x,
                                            mp._y,
                                            mp._v});
            }

            if(maxY)
            {
                _peculiars.emplace_back(PeculiarPoint{
                                            PeculiarPoint::Type::maxY,
                                            mp._x,
                                            mp._y,
                                            mp._v});
            }

            if(maxV)
            {
                _peculiars.emplace_back(PeculiarPoint{
                                            PeculiarPoint::Type::maxV,
                                            mp._x,
                                            mp._y,
                                            mp._v});
            }
        }
    }

    real Line::lastValue() const
    {
        assert(!_points.empty());
        return _points.back()._v;
    }

    real Line::smoothedY() const
    {
        if(_smoothedYSum.size() < 2)
        {
            //assert(0);
            assert(!_points.empty());
            return _points.back()._y;
        }

        return (_smoothedYSum.back() - _smoothedYSum.front())/(_smoothedYSum.size()-1);
    }

    std::size_t Line::endX() const
    {
        return _xStart + _points.size();
    }

    std::size_t Line::size() const
    {
        return _points.size();
    }
}}
