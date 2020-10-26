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
#ifndef CPPLIB_SWAP_RATE_HPP
#define CPPLIB_SWAP_RATE_HPP

/**
***************************************************************************
** Swaps.hpp
***************************************************************************
** Swap analytics.
***************************************************************************
*/

#include <utils/Declare.hpp>
#include <utils/Date.hpp>
#include <utils/DateInterval.hpp>
#include <utils/Holidays.hpp>
#include <utils/Results.hpp>

#include <string>
#include <vector>

CPPLIB_DECLARE_CLASS(IZeroCurve);
CPPLIB_DECLARE_CLASS(CDayCount);
CPPLIB_DECLARE_CLASS(CCashFlowList);

/**
 * Generates the cash flows for a fixed leg of a vanilla interest rate swap.
 */
CCashFlowListConstSP SwapFixedFlows(
    double                  couponRate,
    CDate                   startDate,
    const CDateInterval&    couponInterval,
    CDate                   maturityDate,
    const CDayCountConstSP& dayCount,
    bool                    frontStub,
    bool                    shortStub,
    bool                    subtractInitial,
    bool                    keepStartDate,
    bool                    addFinal,
    EBadDayConv             accBadDayConv,
    EBadDayConv             payBadDayConv,
    const Holidays&         holidays);

/**
 * Old-fashioned routine for calculating the swap rate based on a single
 * swap curve without a separate discount curve or valuation of the floating
 * leg.
 */
double SwapRate(
    const IZeroCurveConstSP& zc,
    CDate startDate,
    const CDateInterval& couponInterval,
    CDate maturityDate,
    const CDayCountConstSP& dayCount,
    EBadDayConv accBadDayConv,
    EBadDayConv payBadDayConv,
    const Holidays& holidays,
    bool stubAtEnd=false);

/* calculates par rate and cash flows for a simple swap */
CMapConstSP SwapAnalytics(
    bool                    getParFlows,
    double                  couponRate,
    CDate                   startDate,
    const CDateInterval&    couponInterval,
    CDate                   maturityDate,
    const CDayCountConstSP& dayCount,
    EBadDayConv             accBadDayConv,
    EBadDayConv             payBadDayConv,
    const Holidays&         holidays,
    const                   IZeroCurveConstSP& zc,
    bool                    stubAtEnd=false);

#endif


