#ifndef CPPLIB_INDEX_CURVE_HPP
#define CPPLIB_INDEX_CURVE_HPP

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

#include "DiscountCurve.hpp"
#include "ZeroCurve.hpp"

#include <utils/DateInterval.hpp>
#include <utils/DayCount.hpp>
#include <utils/Holidays.hpp>

#include <vector>
#include <map>

CPPLIB_DECLARE_STRUCT(CIndexCurveType);
CPPLIB_DECLARE_CLASS(CIndexCurveFixings);
CPPLIB_DECLARE_CLASS(CIndexCurve);

struct CIndexCurveType
{
public:
    CIndexCurveType(
        const std::string& curveName,
        const CDateInterval& rateTenor,
        const CDayCountConstSP& rateDayCount,
        const CDateInterval& fixedSwapInterval,
        const CDayCountConstSP& fixedSwapDayCount,
        const CDateInterval& floatSwapInterval,
        const CDayCountConstSP& floatSwapDayCount,
        EBadDayConv badDayConv,
        const Holidays& holidays);

    const std::string      curveName;
    const CDateInterval    rateTenor;
    const CDayCountConstSP rateDayCount;
    const CDateInterval    fixedSwapInterval;
    const CDayCountConstSP fixedSwapDayCount;
    const CDateInterval    floatSwapInterval;
    const CDayCountConstSP floatSwapDayCount;
    const EBadDayConv      badDayConv;
    const Holidays         holidays;

private:
    void VerifyAndComplete();
};

/*
 * Defines fixings for a particular index curve.
 * The dates are the effective dates for the rates to be fixed.
 */
class CIndexCurveFixings
{
public:
    CIndexCurveFixings(
        const std::vector<CDate>& dates,
        const std::vector<double>& fixings);

    const std::vector<CDate>& Dates() const;
    const std::vector<double>& Rates() const;

    /*
     * Finds the fixing for a given effective date.
     *
     * It is acceptable not to have a fixing for that date when:
     *  1. fixings is NULL.
     *  2. numFixings == 0
     *  3. effectiveDate is after the last date in fixings->fixings
     *
     * However if effectiveDate is on or before the last date in the fixings
     * then we insist that the effectiveDate is found or else we throw an
     * exception.
     */
    void Find(CDate effectiveDate, bool& foundIt, double& fixing) const;

private:

    std::vector<CDate> m_dates;
    std::vector<double> m_rates;
    std::map<CDate, double> m_indexDateRate;
    CDate m_lastFixingDate;

    void VerifyAndComplete();
};

/*
 * Defines an index curve together with recent fixings of the rate.
 */
class CIndexCurve
{
public:
    CIndexCurve(
        const IZeroCurveConstSP& zc,
        const CIndexCurveTypeConstSP& curveType,
        const CIndexCurveFixingsConstSP& curveFixings=CIndexCurveFixingsConstSP());
    
    const IZeroCurveConstSP& ZeroCurve() const;
    const CIndexCurveTypeConstSP& CurveType() const;
    const CIndexCurveFixingsConstSP& CurveFixings() const;

    /*
     * Finds the index rate for a curve and effective date.
     *
     * This can either be a fixed rate if the effective date is on or before
     * the last available fixing date, or an estimated rate from the curve.
     *
     * This is indicated by the rateIsFixed parameter.
     */
    void Rate(CDate effectiveDate,
        bool& rateIsFixed, double& rate);

private:
    IZeroCurveConstSP m_zc;
    CIndexCurveTypeConstSP m_curveType;
    CIndexCurveFixingsConstSP m_curveFixings;

    void VerifyAndComplete();
};

#endif
