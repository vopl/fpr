#pragma once

#include "fpr/math.hpp"
#include "fpr/config.hpp"
#include "fpr/peculiarPoint.hpp"
#include <deque>

namespace fpr { namespace trajectorizer
{
    class Line
    {
    public:
        Line(const Config *config, std::size_t x, std::vector<PeculiarPoint> &peculiars);
        ~Line();

        void step(std::size_t y, real v);

        real lastValue() const;
        real smoothedY() const;
        std::size_t endX() const;
        std::size_t size() const;

    private:
        const Config *_config;
        std::size_t _xStart;

        struct Point
        {
            std::size_t _x;
            std::size_t _y;
            real        _smoothedY;
            real        _v;
        };

        std::deque<Point> _points;
        std::deque<real> _smoothedYSum;

        std::vector<PeculiarPoint> &_peculiars;
    };

}}
