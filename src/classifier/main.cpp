#include <iostream>
#include <vector>
#include "spectra.hpp"
#include "signal.hpp"
#include "harmonicGenerator.hpp"

using namespace tst;



int main(int argc, char *argv[])
{
    std::srand(std::time(0));

    Spectra grid;
    grid.setup(
                50.0, 3000.0, 500,

                30.0, 10.0, 10.0,
                10);

    Signal signal;
    signal.setup(8000, 8000);

    std::vector<HarmonicGenerator *> harmonicGenerators;
    std::random_device src;

    {
        HarmonicGeneratorRandomLaw frequencyLaw, amplitudeLaw;

        frequencyLaw._min = 50;
        frequencyLaw._max = 3000;
        frequencyLaw._timeChangeGenerator = new RandomGeneratorUniform(src, 0.025, 0.01);
        frequencyLaw._valueChangeGenerator = new RandomGeneratorUniform(src, 0, 200);

        amplitudeLaw._min = 0;
        amplitudeLaw._max = 1;
        amplitudeLaw._timeChangeGenerator = new RandomGeneratorUniform(src, 0.125, 0.05);
        amplitudeLaw._valueChangeGenerator = new RandomGeneratorUniform(src, 0, 0.5);


        for(std::size_t i(0); i<10; ++i)
        {
            harmonicGenerators.push_back(new HarmonicGenerator);
            harmonicGenerators.back()->setup(frequencyLaw, amplitudeLaw);
        }
    }

    for(std::size_t frame(1); ; ++frame)
    {
        real time = 0.001 * frame;
        if(time > 10)
        {
            break;
        }

        grid.stepTo(time);
        signal.stepTo(time);

        for( HarmonicGenerator *hg : harmonicGenerators)
        {
            hg->stepTo(time);
            hg->flush(grid);
            hg->flush(signal);
        }


        char fname[1024];
        sprintf(fname, "out/%06d.png", int(frame));
        grid.draw(fname);

        std::cout<<time<<std::endl;
    }


    return 0;
}
