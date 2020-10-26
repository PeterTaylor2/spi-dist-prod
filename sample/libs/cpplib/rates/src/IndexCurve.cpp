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

#include "IndexCurve.hpp"

#include <utils/DateIntervalAdjust.hpp>
#include <utils/RuntimeError.hpp>

/*
**************************************************************************
** Implementation of CIndexCurveType
**************************************************************************
*/
CIndexCurveType::CIndexCurveType(
    const std::string& curveName,
    const CDateInterval& rateTenor,
    const CDayCountConstSP& rateDayCount,
    const CDateInterval& fixedSwapInterval,
    const CDayCountConstSP& fixedSwapDayCount,
    const CDateInterval& floatSwapInterval,
    const CDayCountConstSP& floatSwapDayCount,
    EBadDayConv badDayConv,
    const Holidays& holidays)
    :
    curveName(curveName),
    rateTenor(rateTenor),
    rateDayCount(rateDayCount),
    fixedSwapInterval(fixedSwapInterval),
    fixedSwapDayCount(fixedSwapDayCount),
    floatSwapInterval(floatSwapInterval),
    floatSwapDayCount(floatSwapDayCount),
    badDayConv(badDayConv),
    holidays(holidays)
{
    VerifyAndComplete();
}

void CIndexCurveType::VerifyAndComplete()
{}


/*
**************************************************************************
** Implementation of CIndexCurveFixings
**************************************************************************
*/
CIndexCurveFixings::CIndexCurveFixings(
    const std::vector<CDate>& dates,
    const std::vector<double>& rates)
    :
    m_dates(dates),
    m_rates(rates),
    m_indexDateRate(),
    m_lastFixingDate()
{
    VerifyAndComplete();
}

void CIndexCurveFixings::VerifyAndComplete()
{
    CheckArraySize(m_dates, m_rates, "Dates", "Rates");

    m_indexDateRate.clear();

    size_t nbItems = m_dates.size();

    for (size_t i = 0; i < nbItems; ++i)
    {
        CDate date = m_dates[i];
        double rate = m_rates[i];

        if (m_indexDateRate.count(date) != 0)
            THROW_RUNTIME_ERROR("Duplicate date (" << date.ToString() << ")");

        m_indexDateRate[date] = rate;
        if (date > m_lastFixingDate)
            m_lastFixingDate = date;
    }
}

const std::vector<CDate>& CIndexCurveFixings::Dates() const
{
    return m_dates;
}

const std::vector<double>& CIndexCurveFixings::Rates() const
{
    return m_rates;
}

/*
 * Finds the fixing for a given effective date.
 *
 * It is acceptable not to have a fixing for that date when:
 *  1. fixings is NULL.
 *  2. fixings->numFixings == 0
 *  3. effectiveDate is after the last date in fixings->fixings
 *
 * However if effectiveDate is on or before the last date in fixings->fixings
 * then we insist that the effectiveDate is found or else we return FAILURE.
 *
 * Note that TIndexCurveFixings dates are validated to be in ascending
 * order on construction of the type.
 */
void CIndexCurveFixings::Find(
    CDate effectiveDate,
    bool& foundIt,
    double& fixing) const
{
    foundIt = false;
    fixing = 0.0;

    if (m_indexDateRate.size() == 0)
        return;

    if (effectiveDate > m_lastFixingDate)
        return;

    std::map<CDate,double>::const_iterator iter = m_indexDateRate.find(effectiveDate);
    if (iter == m_indexDateRate.end())
        THROW_RUNTIME_ERROR("Could not find fixing for date " << effectiveDate.ToString());

    foundIt = true;
    fixing = iter->second;
}

/*
**************************************************************************
** Implementation of CIndexCurve
**************************************************************************
*/
CIndexCurve::CIndexCurve(
    const IZeroCurveConstSP& zc,
    const CIndexCurveTypeConstSP& curveType,
    const CIndexCurveFixingsConstSP& curveFixings)
    :
    m_zc(zc),
    m_curveType(curveType),
    m_curveFixings(curveFixings)
{
    VerifyAndComplete();
}

void CIndexCurve::VerifyAndComplete()
{
    PRE_CONDITION(m_zc);
    PRE_CONDITION(m_curveType);
}

const IZeroCurveConstSP& CIndexCurve::ZeroCurve() const
{
    return m_zc;
}

const CIndexCurveTypeConstSP& CIndexCurve::CurveType() const
{
    return m_curveType;
}

const CIndexCurveFixingsConstSP& CIndexCurve::CurveFixings() const
{
    return m_curveFixings;
}


/*
 * Finds the index rate for a curve and effective date.
 *
 * This can either be a fixed rate if the effective date is on or before
 * the last available fixing date, or an estimated rate from the curve.
 *
 * This is indicated by the rateIsFixed parameter.
 */
void CIndexCurve::Rate(
    CDate effectiveDate,
    bool& rateIsFixed,
    double& rate)
{
    try
    {
        m_curveFixings.get()->Find(effectiveDate, rateIsFixed, rate);
        if (rateIsFixed)
            return;

        CDate maturityDate = DateFwdThenAdjust(
            effectiveDate, 
            m_curveType->rateTenor,
            m_curveType->badDayConv,
            m_curveType->holidays);

        if (effectiveDate < m_zc->BaseDate())
        {
            THROW_RUNTIME_ERROR("Effective date " << effectiveDate.ToString()
                << " is before base date of " << m_curveType->curveName
                << " curve " << m_zc->BaseDate().ToString());
        }

        rate = ForwardRate(
            m_zc,
            effectiveDate,
            maturityDate,
            m_curveType->rateDayCount,
            CRateType::SIMPLE);
    }
    catch(std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}
