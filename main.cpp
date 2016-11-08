#include <iostream>
#include "fpr/wavStore.hpp"
#include "fpr/cqt.hpp"
#include "fpr/timeGrid.hpp"
#include "fpr/valueGrid.hpp"
#include "fpr/resultMaker.hpp"

#include <boost/program_options.hpp>

using namespace fpr;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    ////////////////////////////////////////////////////////////////////////////////
     po::options_description desc("fpr");
     desc.add_options()
             ("help", "produce help message")


             ("in-file", po::value<std::string>()->default_value("in.wav"), "input wav file name")

             ("fps", po::value<real>()->default_value(real(8000)), "frames per second analyze")


             ("freq-min", po::value<real>()->default_value(real(100)), "minimum frequency, Hz")
             ("ppw-min", po::value<real>()->default_value(real(10)), "minimum ppw")

             ("freq-max", po::value<real>()->default_value(real(3000)), "maximum frequency, Hz")
             ("ppw-max", po::value<real>()->default_value(real(100)), "maximum ppw")

             ("freq-steps", po::value<std::size_t>()->default_value(400), "frequency grid steps")


             ("find-amax-range", po::value<std::size_t>()->default_value(21), "frequency range for a-maximum find, steps in frequency grid")


             ("time-smooth-range", po::value<real>()->default_value(real(50)), "time range for smooth spectrum, ms")

             ("time-min-step", po::value<real>()->default_value(real(100)), "minimal time range for result step, ms")




             ("value-grid-size", po::value<std::size_t>()->default_value(64), "result dimension")
             ("value-smooth-range", po::value<real>()->default_value(real(50)), "time range for smooth value, ms")
             ("value-bits", po::value<std::size_t>()->default_value(1), "value bitness")

             ("result-format", po::value<std::string>()->default_value("time_value"), "time_value, time_valueElements")

             ("dump-conf", po::value<bool>()->default_value(false), "dump all config values")

             ;



     ////////////////////////////////////////////////////////////////////////////////
     po::variables_map vars;
     try
     {
         po::store(po::command_line_parser(argc, argv).options(desc).run(), vars);
     }
     catch(std::exception &e)
     {
         std::cerr<<e.what()<<std::endl;
         return EXIT_FAILURE;
     }
     catch(...)
     {
         std::cerr<<"unknown exception"<<std::endl;
         return EXIT_FAILURE;
     }
     po::notify(vars);

     /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
     if(vars.count("help"))
     {
 #ifndef GIT_COMMIT
 #   define GIT_COMMIT unknown
 #endif
 #define STR(x) STR2(x)
 #define STR2(x) #x
         std::cout << "git: "<<STR(GIT_COMMIT) << std::endl;
         std::cout << "float bitness: "<<sizeof(real) << std::endl;
         std::cout << desc << std::endl;
         return EXIT_SUCCESS;
     }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    WavStore wavStore(vars["in-file"].as<std::string>().c_str());

    if(!wavStore.good())
    {
        //std::cerr<<"bad wav file: "<<vars["in-file"].as<std::string>()<<std::endl;
        return EXIT_FAILURE;
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Config config;
    config._signalFrequency = wavStore.header()._frequency;
    config._signalBucketSize = std::size_t(real(config._signalFrequency)/vars["fps"].as<real>() + real(0.5));


    {
        std::size_t steps = vars["freq-steps"].as<std::size_t>();
        config._cqtFrequencyGrid.resize(steps);
        config._cqtPpwGrid.resize(steps);
        real min = /*fpr::log*/(vars["freq-min"].as<real>());
        real max = /*fpr::log*/(vars["freq-max"].as<real>());
        real step = (max - min) / (steps - 1);

        for(std::size_t k(0); k<steps; k++)
        {
            config._cqtFrequencyGrid[k] = /*fpr::exp*/(min + k*step);

            real x01 = real(k)/(steps-1);
            config._cqtPpwGrid[k] = vars["ppw-min"].as<real>() * (real(1.0)-x01) + vars["ppw-max"].as<real>()*(x01);
        }
    }

    config._trajectorizerFindMaxHeight = vars["find-amax-range"].as<std::size_t>();



    config._timeGridSmoothRange = vars["time-smooth-range"].as<real>();
    config._timeGridMinStep = vars["time-min-step"].as<real>();




    config._valueGridSize = vars["value-grid-size"].as<std::size_t>();
    config._valueGridSmoothWidth = vars["value-smooth-range"].as<real>();
    config._valuesBits = vars["value-bits"].as<std::size_t>();


    ResultFormat resultFormat;

    if("time_value" == vars["result-format"].as<std::string>())
    {
        resultFormat = ResultFormat::time_value;

    }
    else if("time_valueElements" == vars["result-format"].as<std::string>())
    {
        resultFormat = ResultFormat::time_valueElements;
    }
    else
    {
        std::cerr<<"bad result-format value"<<std::endl;
        return EXIT_FAILURE;
    }



    if(vars["dump-conf"].as<bool>())
    {
        std::cout<<"input file                  : "<<vars["in-file"].as<std::string>()<<std::endl;

        std::cout<<"input frequency             : "<<wavStore.header()._frequency<<std::endl;
        std::cout<<"input bitness               : "<<wavStore.header()._bitness<<std::endl;
        std::cout<<"input length                : "<<real(wavStore.header()._samplesAmount)/wavStore.header()._frequency<<std::endl;



        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        //std::cout<<"signalFrequency             : "<<config._signalFrequency<<std::endl;
        std::cout<<"signalBucketSize            : "<<config._signalBucketSize<<std::endl;


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        std::cout<<"cqtFrequencyGrid min        : "<<config._cqtFrequencyGrid.front()<<std::endl;
        std::cout<<"cqtFrequencyGrid max        : "<<config._cqtFrequencyGrid.back()<<std::endl;
        std::cout<<"cqtFrequencyGrid size       : "<<config._cqtFrequencyGrid.size()<<std::endl;

        std::cout<<"cqtPpwGrid min              : "<<config._cqtPpwGrid.front()<<std::endl;
        std::cout<<"cqtPpwGrid max              : "<<config._cqtPpwGrid.back()<<std::endl;
        std::cout<<"cqtPpwGrid size             : "<<config._cqtPpwGrid.size()<<std::endl;


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        std::cout<<"trajectorizerFindMaxHeight  : "<<config._trajectorizerFindMaxHeight<<std::endl;


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        std::cout<<"timeGridSmoothRange         : "<<config._timeGridSmoothRange<<std::endl;
        std::cout<<"timeGridMinStep             : "<<config._timeGridMinStep<<std::endl;


        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        std::cout<<"valueGridSize               : "<<config._valueGridSize<<std::endl;
        std::cout<<"valueGridSmoothWidth        : "<<config._valueGridSmoothWidth<<std::endl;

        std::cout<<"valuesBits                  : "<<config._valuesBits<<std::endl;



        std::cout<<"resultFormat                : ";
        switch(resultFormat)
        {
        case ResultFormat::time_value:
            std::cout<<"time_value";
            break;
        case ResultFormat::time_valueElements:
            std::cout<<"time_valueElements";
            break;
        }
        std::cout<<std::endl;

        return EXIT_SUCCESS;
    }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Cqt cqt(&config);
    TimeGrid timeGrid(&config);
    ValueGrid valueGrid(&config);
    ResultMaker resultMaker(&config, resultFormat);







    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    {
        TVReal signalBucket(config._signalBucketSize);

        TVReal echo(config._cqtFrequencyGrid.size());
        TVReal values(config._valueGridSize);


        std::size_t framesAmount = wavStore.header()._samplesAmount / config._signalBucketSize;

        for(std::size_t frame(0); frame<framesAmount; ++frame)
        {
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
