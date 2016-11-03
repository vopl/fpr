#pragma once

#include "fpr/config.hpp"
#include "fpr/peculiarPoint.hpp"

namespace fpr
{
    class Grid
    {
    public:
        Grid(const Config *config);
        ~Grid();

        void enplacePeculiars(const PeculiarPoint *pps, std::size_t amount);
        void enplaceEcho(const real *echo);

    private:
        const Config *_config;
    };

}
