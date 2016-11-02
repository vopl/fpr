#include <iostream>
#include "fpr/wavStore.hpp"
#include "fpr/cqt.hpp"
#include "fpr/trajectorizer.hpp"

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


    std::size_t trajectorizerSmoothFilterWidth = 11;
    std::size_t trajectorizerSmoothFilterHeight = 3;
    config._trajectorizerFindMaxHeight = 5;

    config._trajectorizerSmoothFilter.resize(trajectorizerSmoothFilterWidth);
    for(std::size_t x(0); x<trajectorizerSmoothFilterWidth; ++x)
    {
        real hsigma = real(trajectorizerSmoothFilterWidth)/4;
        real hmu = trajectorizerSmoothFilterWidth/2;
        real hv = fpr::exp(-(x-hmu)*(x-hmu)/2/hsigma/hsigma) / hsigma / fpr::sqrt(g_2pi);

        config._trajectorizerSmoothFilter[x].resize(trajectorizerSmoothFilterHeight);
        for(std::size_t y(0); y<trajectorizerSmoothFilterHeight; ++y)
        {
            real vsigma = real(trajectorizerSmoothFilterHeight)/4;
            real vmu = trajectorizerSmoothFilterHeight/2;
            real vv = fpr::exp(-(y-vmu)*(y-vmu)/2/vsigma/vsigma) / vsigma / fpr::sqrt(g_2pi);

            config._trajectorizerSmoothFilter[x][y] = hv*vv;
        }
    }


    {
        std::size_t steps = 400;
        config._frequencyGrid.resize(steps);
        config._ppwGrid.resize(steps);
        real min = /*fpr::log*/(100.0);
        real max = /*fpr::log*/(3000.0);
        real step = (max - min) / (steps - 1);

        for(std::size_t k(0); k<steps; k++)
        {
            config._frequencyGrid[k] = /*fpr::exp*/(min + k*step);

            real x01 = real(k)/(steps-1);
            config._ppwGrid[k] = 4 * (1.0-x01) + 40*(x01);
        }
    }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Cqt cqt(&config);
    Trajectorizer trajectorizer(&config);







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

            trajectorizer.pushSource(&echo[0]);
            trajectorizer.fillResult(&echo[0]);

            for(const real &e : echo)
            {
                echoOut<<e<<", ";
            }
            echoOut<<std::endl;
        }

    }

    return 0;
}
