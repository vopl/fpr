#include "fpr/cqt.hpp"
#include "fpr/cqt/level.hpp"

namespace fpr
{
    Cqt::Cqt(const Config *config)
        : _config(config)
    {
        _levels.reserve(_config->_cqtFrequencyGrid.size());
        for(std::size_t i(0); i<_config->_cqtFrequencyGrid.size(); ++i)
        {
            _levels.emplace_back(new cqt::Level{
                                     _config,
                                     _config->_cqtFrequencyGrid[i],
                                     _config->_cqtPpwGrid[i]});
        }
    }

    Cqt::~Cqt()
    {

    }

    void Cqt::pushSignalBlock(const real *samples)
    {
        for(auto &l : _levels)
        {
            l->pushSignalBlock(samples, _signalBucketsPushed);
        }

        _signalBucketsPushed++;
    }

    void Cqt::fillEcho(complex *values)
    {
        for(auto &l : _levels)
        {
            *values = l->getEcho();
            values++;
        }
    }

    void Cqt::fillEchoA(real *values)
    {
        for(auto &l : _levels)
        {
            *values = l->getEchoA();
            values++;
        }
    }

    void Cqt::fillEchoA2(real *values)
    {
        for(auto &l : _levels)
        {
            *values = l->getEchoA2();
            values++;
        }
    }

}
