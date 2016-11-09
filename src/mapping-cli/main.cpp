#include <iostream>
#include "mapping/solver.hpp"
#include <boost/program_options.hpp>

using namespace aid;
using namespace aid::mapping;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    ////////////////////////////////////////////////////////////////////////////////
     po::options_description desc("mapping");
     desc.add_options()
             ("help", "produce help message")



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
         std::cout << desc << std::endl;
         return EXIT_SUCCESS;
     }


     Solver solver;


     //224
     solver.pushFingerprintValue(0.306, 3662391059158792808);
     solver.pushFingerprintValue(0.466, 153767746745509228);
     solver.pushFingerprintValue(1.13 , 76997443224478480);
     solver.pushFingerprintValue(1.39 , 1134694899700597976);
     solver.pushFingerprintValue(1.7  , 9298612641484232);
     solver.pushFingerprintValue(2.028, 165675728155255372);
     solver.pushFingerprintValue(2.362, 71026161640520);
     solver.pushFingerprintValue(2.614, 38564291394283080);
     solver.pushFingerprintValue(2.784, 2474923856173572812);
     solver.pushFingerprintValue(2.928, 905826680818930276);
     solver.pushFingerprintValue(3.18 , 230612393461360204);
     solver.pushFingerprintValue(3.36 , 9879238212);
     solver.pushFingerprintValue(3.488, 1152941452908337444);
     solver.pushFingerprintValue(3.748, 9871194274804496);
     solver.pushFingerprintValue(4.098, 2650536877789243080);
     solver.pushFingerprintValue(4.456, 10577405128);
     solver.pushFingerprintValue(4.646, 4569587208389572046);
     solver.pushFingerprintValue(4.89 , 1125899982938692);
     solver.pushFingerprintValue(5.09 , 144115188075901004);
     solver.pushFingerprintValue(5.37 , 158189081679580598);
     solver.pushFingerprintValue(6.026, 184719681325968202);
     solver.pushFingerprintValue(6.244, 164698329913065152);
     solver.pushFingerprintValue(6.426, 144788201619679936);
     solver.pushFingerprintValue(6.624, 154046261330803530);
     solver.pushFingerprintValue(6.804, 144713675336919872);

     //solver.pushFingerprintValue()

    return 0;
}
