#pragma once

#include "fpr/config.hpp"

namespace fpr
{
    class ResultMaker
    {
    public:
        ResultMaker(const Config *config);
        ~ResultMaker();

        void push(std::size_t pos, real *values);

    private:
        const Config *_config;

    };
}
