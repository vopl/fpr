#include "fpr/resultMaker.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>

namespace fpr
{
    ResultMaker::ResultMaker(const Config *config)
        : _config(config)
    {

    }

    ResultMaker::~ResultMaker()
    {

    }

    void ResultMaker::push(std::size_t pos, real *values)
    {
        static std::ofstream out("vs");

        for(std::size_t i(0); i<_config->_valueGridSize; ++i)
        {
            assert(values[i] >= 0 && values[i]<=1);

            unsigned sv = (unsigned)(values[i]* (1u << (_config->_valuesBits-1)) + 0.5);

            out<<sv<<", ";
        }
        out<<std::endl;

        std::cout<<"      flush: "<<pos<<std::endl;
    }
}
