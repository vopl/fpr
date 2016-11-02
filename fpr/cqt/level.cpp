#include "fpr/cqt/level.hpp"

namespace fpr { namespace cqt
{
    Level::Level(const Config *config, real frequency, real ppw)
        : _config(config)
        , _frequency(frequency)
    {
        real length = ppw / _frequency;
        std::size_t serieSize = std::size_t(length*_config->_signalFrequency + 0.5);
        _echoSerie.resize(serieSize);
    }

    Level::~Level()
    {

    }

    namespace
    {
        template<typename T>
        inline T fastCos01(T x) noexcept
        {
//            static const T tp = 1./(2.*M_PI);
//            x *= tp;
            x -= T(.25) + std::floor(x + T(.25));
            x *= T(16.) * (std::abs(x) - T(.5));
            #if EXTRA_PRECISION
            x += T(.225) * x * (std::abs(x) - T(1.));
            #endif
            return x;
        }

        template<typename T>
        inline T fastSin01(T x) noexcept
        {
            return fastCos01(x-T(.25));
        }

    }

    void Level::pushSignalBlock(const real *samples, std::size_t bucketsAlreadyPushed)
    {
        complex sum = _echoSerie.back();

        real x0 = real(_config->_signalBucketSize * bucketsAlreadyPushed) / _config->_signalFrequency;

        for(std::size_t i(0); i<_config->_signalBucketSize; ++i)
        {
            real x = x0 + real(i) / _config->_signalFrequency;
            real v = samples[i];


            //real dp = x*g_2pi*_frequency;
            //sum += complex(v, 0).rotate(x*g_2pi*_frequency);

            real dp01 = x*_frequency;

            sum += complex(v*fastCos01(dp01), v*fastSin01(dp01));

            _echoSerie.push_back(sum);
        }
    }

    complex Level::getEcho()
    {
        auto cv = (_echoSerie.back() - _echoSerie.front()) / real(_echoSerie.size());
        return cv;
    }

    real Level::getEchoA()
    {
        auto cv = (_echoSerie.back() - _echoSerie.front()) / real(_echoSerie.size());
        return cv.a();
    }

    real Level::getEchoA2()
    {
        auto cv = (_echoSerie.back() - _echoSerie.front()) / real(_echoSerie.size());
        return cv.a2();
    }


}}
