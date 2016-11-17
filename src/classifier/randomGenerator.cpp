#include "randomGenerator.hpp"

namespace tst
{
    RandomGenerator::~RandomGenerator()
    {

    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    RandomGeneratorUniform::RandomGeneratorUniform(std::random_device &src, real center, real width)
        : _src(src)
        , _distr(center - width/2, center + width/2)
    {
    }

    real RandomGeneratorUniform::generate()
    {
        return _distr(_src);
    }

}
