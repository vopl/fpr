#include "spectra.hpp"
#define cimg_display 0
#include <CImg.h>

namespace tst
{
    Spectra::Spectra()
    {

    }

    Spectra::~Spectra()
    {

    }


    void Spectra::setup(
            real fMin, real fMax,
            std::size_t fSteps,

            real periodsBefore, real periodsInTarget, real periodsAfter,
            std::size_t samplesPerPeriod)
    {
        _fMin = fMin;
        _fMax = fMax;
        _fSteps=fSteps;
        _fStep = (_fMax - _fMin)/_fSteps;

        _periodsBefore = periodsBefore;
        _periodsInTarget = periodsInTarget;
        _periodsAfter = periodsAfter;

        _samplesPerPeriod = samplesPerPeriod;

        _levels.clear();
        _levels.reserve(_fSteps);
        for(std::size_t i(0); i<_fSteps; ++i)
        {
            real f = _fMin + real(i) * _fStep + _fStep/2;
            _levels.emplace_back(new spectra::Level(f, _fStep, _periodsBefore, _periodsInTarget, _periodsAfter, _samplesPerPeriod));

        }
    }


    void Spectra::stepTo(real time)
    {
        for(auto &level : _levels)
        {
            level->stepTo(time);
        }
    }


    void Spectra::pushHarmonicSegment(
            real t0, real t1,
            real f0, real f1,
            real a0, real a1)
    {
        int levelIndex0 = int((f0 - _fMin)/_fStep);
        int levelIndex1 = int((f1 - _fMin)/_fStep);

        if(levelIndex0 > levelIndex1)
        {
            std::swap(levelIndex0, levelIndex1);
        }

        levelIndex0 = std::max(0, levelIndex0-1);
        levelIndex1 = std::min(int(_fSteps), levelIndex1+2);

        for(int levelIndex(levelIndex0); levelIndex < levelIndex1; ++levelIndex)
        {
            _levels[std::size_t(levelIndex)]->pushHarmonicSegment(t0, t1, f0, f1, a0, a1);
        }
        return;


        static std::ofstream out("out");

        out.precision(14);
        out.setf(std::ios::scientific);

        out<<t0<<", "<<f0<<", "<<a0<<std::endl;


        //assert(0);
    }

    real &Spectra::data(std::size_t levelIdx, std::size_t sampleIdx)
    {
        return _levels[levelIdx]->data(sampleIdx);
    }

    void Spectra::draw(const char *fname)
    {
        using namespace cimg_library;
        cimg::exception_mode(1);

        std::size_t width = _levels[0]->dataSize();
        std::size_t height = _levels.size();

        CImg<unsigned char> visu(width, height);
        visu = real(0);

        real max = 0;
        for(std::size_t y(0); y < height; ++y)
        {
            for(std::size_t x(0); x < width; ++x)
            {
                max = std::max(_levels[y]->data(x), max);
            }
        }

        if(max>0)
        {
            for(std::size_t y(0); y < height; ++y)
            {
                for(std::size_t x(0); x < width; ++x)
                {
                    real v = _levels[y]->data(x) / max * 255;

                    if(v > 0)
                    {
                        visu(x, height-1-y) = v;
                    }
                }
            }
        }

        visu.save(fname);


        //assert(0);
    }


}
