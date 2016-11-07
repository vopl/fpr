#include <iostream>
#include "fpr/wavStore.hpp"
#include "fpr/cqt.hpp"
#include "fpr/timeGrid.hpp"
#include "fpr/valueGrid.hpp"
#include "fpr/resultMaker.hpp"

using namespace fpr;

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    WavStore wavStore("/work/tmp/wav/med_8.wav");

    if(!wavStore.good())
    {
        std::cerr<<"bad wav file"<<std::endl;
        return EXIT_FAILURE;
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Config config;
    config._signalFrequency = wavStore.header()._frequency;
    config._signalBucketSize = 1;//config._signalFrequency/8000;//1000 frames per second


    {
        std::size_t steps = config._valueGridSize*5;
        config._cqtFrequencyGrid.resize(steps);
        config._cqtPpwGrid.resize(steps);
        real min = /*fpr::log*/(100.0);
        real max = /*fpr::log*/(3000.0);
        real step = (max - min) / (steps - 1);

        for(std::size_t k(0); k<steps; k++)
        {
            config._cqtFrequencyGrid[k] = /*fpr::exp*/(min + k*step);

            real x01 = real(k)/(steps-1);
            config._cqtPpwGrid[k] = 10 * (1.0-x01) + 100*(x01);
        }
    }

    config._trajectorizerFindMaxHeight = config._cqtFrequencyGrid.size()/16;


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Cqt cqt(&config);
    TimeGrid timeGrid(&config);
    ValueGrid valueGrid(&config);
    ResultMaker resultMaker(&config);







    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    {
        TVReal signalBucket(config._signalBucketSize);

        TVReal echo(config._cqtFrequencyGrid.size());
        TVReal values(config._valueGridSize);


        std::size_t framesAmount = wavStore.header()._samplesAmount / config._signalBucketSize;

        for(std::size_t frame(0); frame<framesAmount; ++frame)
        {
            //std::cerr<<frame<<std::endl;
            if(!wavStore.read(&signalBucket[0], signalBucket.size()))
            {
                break;
            }

            cqt.pushSignalBlock(&signalBucket[0]);
            cqt.fillEchoA(&echo[0]);

            valueGrid.enplaceEcho(&echo[0]);

            bool hasPeculiar;
            timeGrid.enplaceEcho(&echo[0], hasPeculiar);
            if(hasPeculiar)
            {
                valueGrid.flush(timeGrid.lastPeculiarPosition(), &values[0]);
                resultMaker.push(timeGrid.lastPeculiarPosition(), &values[0]);
            }
        }

    }

    return 0;
}
