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
#ifndef CPPLIB_ROOT_BRENT_HPP
#define CPPLIB_ROOT_BRENT_HPP

/**
***************************************************************************
** RootBrent.hpp
***************************************************************************
** Implements the Brent root solver in C++.
***************************************************************************
*/

/**
 * Defines a function of one variable
 */
class CFunction1
{
public:
    virtual ~CFunction1() {}
    virtual double operator()(double x) const = 0;
};

/**
 * Finds the root of f(x) = target using a combination of secant, bisection
 * and an inverse quadratic interpolation method.
 */
double RootFindBrent(
    const CFunction1& function,
    double target,
    double boundLo,
    double boundHi,
    int    numIterations,
    double guess,
    double xAccuracy,
    double fAccuracy,
    double initialXStep = 0.0,
    double initialFDeriv = 0.0);

#endif


