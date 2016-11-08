#include "fpr/resultMaker.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>

namespace fpr
{
    ResultMaker::ResultMaker(const Config *config, ResultFormat format)
        : _config(config)
        , _format(format)
    {

    }

    ResultMaker::~ResultMaker()
    {

    }

    void ResultMaker::push(std::size_t pos, real *values)
    {
        std::cout<<real(pos)*_config->_signalBucketSize/_config->_signalFrequency<<": ";

        switch(_format)
        {
        case ResultFormat::time_value:
            {
                std::uint64_t res(0);
                for(std::size_t i(0); i<_config->_valueGridSize; ++i)
                {
                    assert(values[i] >= 0 && values[i]<=1);

                    std::uint64_t sv = (std::uint64_t)(values[i]* (1u << (_config->_valuesBits-1)) + 0.5);

                    res |= (sv << (i*_config->_valuesBits));
                }
                std::cout<<res;
            }
            break;
        case ResultFormat::time_valueElements:
            for(std::size_t i(0); i<_config->_valueGridSize; ++i)
            {
                assert(values[i] >= 0 && values[i]<=1);

                if(i)
                {
                    std::cout<<", ";
                }

                unsigned sv = (unsigned)(values[i]* (1u << (_config->_valuesBits-1)) + 0.5);

                std::cout<<sv;
            }
            break;
        }

        std::cout<<std::endl;
    }
}
