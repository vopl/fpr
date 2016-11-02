#pragma once

#include <cmath>
#include <limits>
#include <vector>

namespace fpr
{
    using real = float;
    //using real = double;


    using TVReal = std::vector<real>;


    static const real g_pi = real(M_PI);
    static const real g_2pi = real(M_PI*2);


    /////////-/////////-/////////-/////////-/////////-/////////-/////////-/////////-
    inline double fabs(double x) {return ::fabs(x);}
    inline double sqrt(double x) {return ::sqrt(x);}

    inline double cos(double x) {return ::cos(x);}
    inline double sin(double x) {return ::sin(x);}

    inline double exp(double x) {return ::exp(x);}
    inline double log(double x) {return ::log(x);}

    inline double atan2(double x, double y) {return ::atan2(x, y);}

    /////////-/////////-/////////-/////////-/////////-/////////-/////////-/////////-
    inline long double fabs(long double x) {return ::fabsl(x);}
    inline long double sqrt(long double x) {return ::sqrtl(x);}

    inline long double cos(long double x) {return ::cosl(x);}
    inline long double sin(long double x) {return ::sinl(x);}

    inline long double exp(long double x) {return ::expl(x);}
    inline long double log(long double x) {return ::logl(x);}

    inline long double atan2(long double x, long double y) {return ::atan2l(x, y);}


}

#include "fpr/complex.hpp"

namespace fpr
{
    using complex = complex_tmpl<real>;
    //////////////////////////////////////////////////////////////////////////
    typedef std::vector<complex> TVComplex;
}
