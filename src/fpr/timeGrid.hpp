#pragma once

#include "config.hpp"
#include <boost/circular_buffer.hpp>

namespace aid { namespace fpr
{
    class TimeGrid
    {
    public:
        TimeGrid(const Config *config);
        ~TimeGrid();

        void enplaceEcho(const real *echo, bool &hasPeculiar);

        std::size_t lastPeculiarPosition();

    private:
        const Config *_config;

        std::size_t _echosPushed = 0;

        boost::circular_buffer<real> _echoSum;
        boost::circular_buffer<real> _echoSmoothed;

        real maxValue = 0;
        std::size_t maxPos = 0;

        //std::deque<std::size_t> _foundMaxPositions;
        std::size_t _lastPeculiarPosition;

    };

}}
