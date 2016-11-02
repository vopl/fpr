#include <iostream>
#include "fpr/wavStore.hpp"
#include "fpr/cqt.hpp"

using namespace fpr;

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    WavStore wavStore("/work/tmp/wav/in_8.wav");

    if(!wavStore.good())
    {
        std::cerr<<"bad wav file"<<std::endl;
        return EXIT_FAILURE;
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Config config;
    config._signalFrequency = wavStore.header()._frequency;
    config._signalBucketSize = config._signalFrequency/1000;//fps=1000


    {
        std::size_t steps = 100;
        config._frequencyGrid.resize(steps);
        config._ppwGrid.resize(steps);
        real min = /*fpr::log*/(100.0);
        real max = /*fpr::log*/(3000.0);
        real step = (max - min) / (steps - 1);

        for(std::size_t k(0); k<steps; k++)
        {
            config._frequencyGrid[k] = /*fpr::exp*/(min + k*step);

            real x01 = real(k)/(steps-1);
            config._ppwGrid[k] = 5 * (1.0-x01) + 50*(x01);
        }
    }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Cqt cqt(&config);







    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    {
        TVReal signalBucket(config._signalBucketSize);

        TVReal echo(config._frequencyGrid.size());
        std::ofstream echoOut("ea2");


        std::size_t framesAmount = wavStore.header()._samplesAmount / config._signalBucketSize;

        for(std::size_t frame(0); frame<framesAmount; ++frame)
        {
            std::cerr<<frame<<std::endl;
            if(!wavStore.read(&signalBucket[0], signalBucket.size()))
            {
                break;
            }

            cqt.pushSignalBlock(&signalBucket[0]);
            cqt.fillEchoA(&echo[0]);

            for(const real &e : echo)
            {
                echoOut<<e<<", ";
            }
            echoOut<<std::endl;
        }

    }

    return 0;
}
