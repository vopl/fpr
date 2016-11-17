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
        frequencyLaw._timeChangeGenerator = new RandomGeneratorUniform(src, 0.25, 0.1);
        frequencyLaw._valueChangeGenerator = new RandomGeneratorUniform(src, 0, 100);

        amplitudeLaw._min = 0;
        amplitudeLaw._max = 1;
        amplitudeLaw._timeChangeGenerator = new RandomGeneratorUniform(src, 0.25, 0.1);
        amplitudeLaw._valueChangeGenerator = new RandomGeneratorUniform(src, 0, 0.01);


        harmonicGenerators.push_back(new HarmonicGenerator);
        harmonicGenerators.back()->setup(frequencyLaw, amplitudeLaw);
    }

    for(real time(0.01); time<10; time+=0.01)
    {
        grid.stepTo(time);
        //signal.stepTo(time);

        for( HarmonicGenerator *hg : harmonicGenerators)
        {
            hg->stepTo(time);
            hg->flush(grid);
            //hg.flush(signal);
        }


        grid.draw("out.png");

        std::cout<<time<<std::endl;
    }


    return 0;
}
