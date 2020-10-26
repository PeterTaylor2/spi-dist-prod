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

#ifndef CPPLIB_ZC_BUILD_HPP
#define CPPLIB_ZC_BUILD_HPP

#include "ZeroCurve.hpp"
#include <utils/DateInterval.hpp>
#include <utils/Holidays.hpp>


/*
***************************************************************************
** Creates a zero curve from cash points.
***************************************************************************
*/
CZeroCurveConstSP ZeroCurveCash(
    CDate                      baseDate,
    const std::vector<CDate>&  dates,
    const std::vector<double>& rates,
    const CDayCountConstSP&    dayCount,
    CZeroCurve::EInterpType    interpType);

/*
***************************************************************************
** Adds swap points to a given zero curve. Uses the interpolation type
** of the original curve. One-curve model.
***************************************************************************
*/
CZeroCurveConstSP ZeroCurveSwaps(
    const CZeroCurveConstSP&   stubCurve,
    const std::vector<CDate>&  dates,
    const std::vector<double>& rates,
    const CDateInterval&       couponInterval,
    const CDayCountConstSP&    dayCountConv,
    EBadDayConv                badDayConv,
    const Holidays&            holidays);

#endif 
