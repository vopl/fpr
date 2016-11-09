#pragma once

#include "config.hpp"
#include "math.hpp"
#include <memory>

namespace aid { namespace fpr
{
    namespace cqt
    {
        class Level;
    }

    class Cqt
    {
    public:
        Cqt(const Config *config);
        ~Cqt();

        void pushSignalBlock(const real *samples);
        void fillEcho(complex *values);
        void fillEchoA(real *values);
        void fillEchoA2(real *values);

    private:
        const Config *_config;

        std::vector<std::unique_ptr<cqt::Level>> _levels;
        std::size_t _signalBucketsPushed = 0;
    };
}}
