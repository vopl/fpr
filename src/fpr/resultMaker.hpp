#pragma once

#include "config.hpp"

namespace aid { namespace fpr
{
    enum class ResultFormat
    {
        time_valueElements,
        time_value,
    };

    class ResultMaker
    {
    public:
        ResultMaker(const Config *config, ResultFormat format);
        ~ResultMaker();

        void push(std::size_t pos, real *values);

    private:
        const Config *_config;
        ResultFormat _format;

    };
}}
