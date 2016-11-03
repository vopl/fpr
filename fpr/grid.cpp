#include "fpr/grid.hpp"

namespace fpr
{
    Grid::Grid(const Config *config)
        : _config(config)
    {

    }

    Grid::~Grid()
    {

    }

    void Grid::enplacePeculiars(const PeculiarPoint *pps, std::size_t amount)
    {
        assert(0);
    }

    void Grid::enplaceEcho(const real *echo)
    {
        assert(0);
    }

}
