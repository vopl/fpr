#pragma once
#include "common.hpp"
#include <random>

namespace tst
{
    class RandomGenerator
    {
    public:
        virtual ~RandomGenerator();
        virtual real generate() = 0;

    };

    class RandomGeneratorUniform
            : public RandomGenerator
    {
    public:
        RandomGeneratorUniform(std::random_device &src, real center, real width);
        virtual real generate() override;

    private:
        std::random_device &_src;
        std::uniform_real_distribution<real> _distr;

    };
}
