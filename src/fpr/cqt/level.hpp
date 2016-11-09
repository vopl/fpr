#pragma once

#include "config.hpp"
#include <boost/circular_buffer.hpp>

namespace aid { namespace fpr { namespace cqt
{
    class Level
    {
    public:
        Level(const Config *config, real frequency, real ppw);
        ~Level();

        void pushSignalBlock(const real *samples, std::size_t bucketsAlreadyPushed);
        complex getEcho();
        real getEchoA();
        real getEchoA2();

    private:
        const Config *_config;
        real _frequency;

        boost::circular_buffer<complex> _echoSerie;
    };

}}}
