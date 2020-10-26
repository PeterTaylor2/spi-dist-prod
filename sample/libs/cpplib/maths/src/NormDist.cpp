/*

Copyright (C) 2015 Sartorial Programming Ltd.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

/**
***************************************************************************
** NormDist.cpp
***************************************************************************
** Functions which deal with the normal distribution.
***************************************************************************
*/

#include "NormDist.hpp"
#include "Constants.hpp"
#include <utils/RuntimeError.hpp>
#include <utils/RootBrent.hpp>

#include <math.h>

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

#define SIGN(x) ((x) >= 0.0 ? 1.0 : -1.0)

BEGIN_ANONYMOUS_NAMESPACE
double CBND_HELPER(double a1, double b1, double rho)
{
    const double x[5] = {
        0.24840615,
        0.39233107,
        0.21141819,
        0.033246660,
        0.00082485334};

    const double y[5] = {
        0.10024215,
        0.48281397,
        1.0609498,
        1.7797294,
        2.6697604};

    PRE_CONDITION(a1 <= 0.0);
    PRE_CONDITION(b1 <= 0.0);
    PRE_CONDITION(rho <= 0.0);

    double sum = 0.0;

    for (size_t i = 0; i < 5; ++i)
    {
        for (size_t j = 0; j < 5; ++j)
        {
            double arg = a1 * (2 * y[i] - a1) + b1 * (2 * y[j] - b1) +
            2 * rho * (y[i] - a1) * (y[j] - b1);
            double sumand = x[i] * x[j] * exp(arg);
            sum += sumand;
        }
    }
    double phi = sqrt(1.0 - rho*rho) / PI * sum;

    return phi;
}

class CND_FUNCTION : public CFunction1
{
    double operator()(double x) const
    {
        return NormDist::CND(x);
    }
};

END_ANONYMOUS_NAMESPACE

double NormDist::PHI(double x)
{
    return exp(-x * x * 0.5) / SQRT_TWO_PI;
}

double NormDist::CND(double x)
{
    // doubtless we could do better than this eventually...

    const double a1 = 0.31938153;
    const double a2 = -0.356563782;
    const double a3 = 1.781477937;
    const double a4 = -1.821255978;
    const double a5 = 1.330274429;

    double L = fabs(x);

    double k = 1.0 / (1.0 + 0.2316419 * L);
    double result = 1.0 - 1.0 / SQRT_TWO_PI * exp(-L*L/2.0) *
        ((((((a5 * k + a4) * k + a3) * k + a2) * k) + a1) * k);

    if (x < 0.0)
    result = 1.0 - result;

    return result;
}

double NormDist::CND_INVERSE(double prob)
{
    // doubtless we could do better than this eventually...

    PRE_CONDITION(prob <= 1.0);
    PRE_CONDITION(prob >= 0.0);

    double prob1 = 1.0 - prob;
    if (prob1 == 0.0)
        return DBL_MAX;

    if (prob == 0.0)
        return -DBL_MAX;

    CND_FUNCTION function;

    const double boundLo      = -10.0;
    const double boundHi      = 10.0;
    const int maxIterations   = 100;
    const double guess        = 0.0;
    const double initialXStep = 0.0;
    const double xAccuracy    = 1e-7;
    const double fAccuracy    = 1e-7;

    double initialFDeriv = PHI(guess);

    double x = RootFindBrent(
        CND_FUNCTION(),
        prob,
        boundLo,
        boundHi,
        maxIterations,
        guess,
        xAccuracy,
        fAccuracy,
        initialXStep,
        initialFDeriv);

    return x;
}


double NormDist::CBND(double a, double b, double rho)
{
    PRE_CONDITION(rho < 1.0);
    PRE_CONDITION(rho > -1.0);

    double a1 = a / sqrt(2.0 * (1.0 - rho * rho));
    double b1 = b / sqrt(2.0 * (1.0 - rho * rho));

    double result;
    if (a1 <= 0.0 && b1 <= 0.0 && rho <= 0.0)
    {
        result = CBND_HELPER(a1, b1, rho);
    }
    else if (a1 <= 0.0 && b1 >= 0.0 && rho >= 0.0)
    {
        result = CND(a) - CBND_HELPER(a1, -b1, -rho);
    }
    else if (a1 >= 0.0 && b1 <= 0.0 && rho >= 0.0)
    {
        result = CND(b) - CBND_HELPER(-a1, b1, -rho);
    }
    else if (a1 >= 0.0 && b1 >= 0.0 && rho <= 0.0)
    {
        result = CND(a) +
            CND(b) - 1.0 + CBND_HELPER(-a1, -b1, rho);
    }
    else
    {
        POST_CONDITION(a*b*rho > 0.0);
        double rho1 = (rho * a - b) * SIGN(a) / sqrt(a*a + b*b - 2*rho*a*b);
        double rho2 = (rho * b - a) * SIGN(b) / sqrt(a*a + b*b - 2*rho*a*b);
        double delta = (1.0 - SIGN(a) * SIGN(b)) / 4.0;
        // touch of recursion here...
        // since of the inputs is zero we will go down different branches
        result = CBND(a, 0.0, rho1) + CBND(b, 0.0, rho2) - delta;
    }
    return result;
}

