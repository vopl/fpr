#pragma once

#include "config.hpp"
#include <boost/circular_buffer.hpp>
#include <memory>

namespace aid { namespace fpr
{
    class ValueGrid
    {
    public:
        ValueGrid(const Config *config);
        ~ValueGrid();

        void enplaceEcho(const real *echo);

        void flush(std::size_t pos, real *values);

    private:
        real *getEcho(std::size_t pos);

    private:
        const Config *_config;

        std::vector<std::unique_ptr<real[]>> _echo;
        std::size_t _echosPushed = 0;

        std::size_t _smoothWidth;


    };
}}
