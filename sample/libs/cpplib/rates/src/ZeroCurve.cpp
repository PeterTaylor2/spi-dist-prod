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
** ZeroCurve.cpp
***************************************************************************
** Defines a curve of zero coupon rates which can subsequently be used 
** for a discount curve or forward rate curve.
***************************************************************************
*/

#include "ZeroCurve.hpp"
#include "CashFlowList.hpp"

#include <utils/EnumLookup.hpp>
#include <utils/RuntimeError.hpp>
#include <spi_util/Utils.hpp>

#include <math.h>

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_ANONYMOUS_NAMESPACE

void BracketDate(
    const std::vector<CDate>& dates,
    CDate date,
    int& loIndex,
    int& hiIndex,
    int& exactIndex)
{
    PRE_CONDITION(dates.size() > 0);

    // looking for index of first entry in dates >= date, i.e. not less than, i.e. lower_bound
    std::vector<CDate>::const_iterator iter = 
        std::lower_bound(dates.begin(), dates.end(), date);

    if (iter == dates.end())
    {
        POST_CONDITION(date > dates.back());
        loIndex = (int)dates.size() - 1;
        hiIndex = -1;
        exactIndex = -1;
    }
    else if (*iter == date)
    {
        loIndex = -1;
        hiIndex = -1;
        exactIndex = (int)(iter - dates.begin());
    }
    else if (iter == dates.begin())
    {
        POST_CONDITION(date < dates.front());
        loIndex = -1;
        hiIndex = 0;
        exactIndex = -1;
    }
    else
    {
        POST_CONDITION(date > dates.front());
        POST_CONDITION(date < dates.back());
        hiIndex = (int)(iter - dates.begin());
        loIndex = hiIndex - 1;
        exactIndex = -1;
    }
}

END_ANONYMOUS_NAMESPACE

IZeroCurve::~IZeroCurve()
{}

CZeroCurve::CZeroCurve(
    CDate baseDate,
    EInterpType interpType,
    const std::vector<CDate>& dates,
    const std::vector<double>& rates,
    CRateType rateType,
    const CDayCountConstSP& dayCount)
    :
    m_baseDate(baseDate),
    m_interpType(interpType),
    m_dates(dates),
    m_rates(rates),
    m_rateType(rateType),
    m_dayCount(dayCount)
{
    VerifyAndComplete();
}

void CZeroCurve::VerifyAndComplete()
{
    CheckArraySize(m_dates, m_rates, "dates", "rates");

    for (size_t i = 1; i < m_dates.size(); ++i)
    {
        if (m_dates[i] <= m_dates[i-1])
            THROW_RUNTIME_ERROR("Dates are not in ascending order");
    }

    PRE_CONDITION(m_baseDate);
    PRE_CONDITION(m_dayCount);
}

CDate CZeroCurve::BaseDate() const 
{ 
    return m_baseDate; 
}

CZeroCurve::EInterpType CZeroCurve::InterpType() const 
{
    return m_interpType;
}

const std::vector<CDate>& CZeroCurve::Dates() const
{ 
    return m_dates;
}

const std::vector<double>& CZeroCurve::Rates() const
{ 
    return m_rates;
}

CRateType CZeroCurve::RateType() const
{ 
    return m_rateType;
}

const CDayCountConstSP& CZeroCurve::DayCount() const
{
    return m_dayCount;
}

CDate CZeroCurve::FirstDate() const
{
    if (m_dates.size() == 0)
        return m_baseDate;

    return std::min(m_dates.front(), m_baseDate);
}

CDate CZeroCurve::LastDate() const
{
    if (m_dates.size() == 0)
        return m_baseDate;

    return std::max(m_dates.back(), m_baseDate);
}

double CZeroCurve::ZeroRate(CDate interpDate) const
{
    try
    {
        switch(m_interpType)
        {
        case LINEAR:
            return LinearZeroRate(interpDate);
        case FLAT_FORWARDS:
            {
                double time = m_dayCount->Time(m_baseDate, interpDate);
                while (time == 0.0)
                {
                    interpDate += 1;
                    time = m_dayCount->Time(m_baseDate, interpDate);
                }
                double discount = FlatForwardsPV(interpDate);
                double rate = m_rateType.DiscountToRate(discount, time);
                return rate;
            }
            break;
        default:
            THROW_RUNTIME_ERROR("Unsupported interpolation type: " << (int)m_interpType);
        }
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

double CZeroCurve::PV(CDate interpDate) const
{
    try
    {
        switch(m_interpType)
        {
        case LINEAR:
            {
                double zeroRate = LinearZeroRate(interpDate);
                double time = m_dayCount->Time(m_baseDate, interpDate);
                double discount =  m_rateType.RateToDiscount(zeroRate, time);
                return discount;
            }
            break;
        case FLAT_FORWARDS:
            return FlatForwardsPV(interpDate);
        default:
            THROW_RUNTIME_ERROR("Unsupported interpolation type: " << (int)m_interpType);
        }
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

double CZeroCurve::LinearZeroRate(CDate interpDate) const
{
    int lo, hi, exact;
    
    try
    {

        BracketDate(m_dates, interpDate, lo, hi, exact);

        if (exact >= 0)
        {
            POST_CONDITION(exact < (int)m_dates.size());
            return m_rates[exact];
        }

        if (m_rateType.number == CRateType::DISCOUNT_FACTOR)
            THROW_RUNTIME_ERROR("Cannot use linear interpolation on discount factors");

        if (lo < 0)
        {
            POST_CONDITION(hi == 0);
            if (m_dates[0] >= m_baseDate)
                return m_rates[hi];

            THROW_RUNTIME_ERROR("Cannot interpolate before start of curve " << m_dates[0]
                << " which is also before base date " << m_baseDate);

        }
        if (hi < 0)
        {
            POST_CONDITION(lo+1 == (int)m_rates.size());
            return m_rates[lo];
        }

        double r1 = m_rates[lo];
        double r2 = m_rates[hi];
        double t1 = m_dayCount->Time(m_baseDate, m_dates[lo]);
        double t2 = m_dayCount->Time(m_baseDate, m_dates[hi]);
        double t = m_dayCount->Time(m_baseDate, interpDate);
            
        POST_CONDITION(t2 >= t1);

        if (t2 == t1)
            return (r1+r2)/2;

        double rate = r1 + (r2-r1) * (t - t1) / (t2 - t1);
        return rate;
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

double CZeroCurve::FlatForwardsPV(CDate interpDate) const
{
    int lo, hi, exact;
    
    try
    {
        BracketDate(m_dates, interpDate, lo, hi, exact);

        if (exact >= 0)
        {
            POST_CONDITION(exact < (int)m_dates.size());
            return RateToDiscount(
                m_rates[exact], m_baseDate, m_dates[exact], m_dayCount, m_rateType);
        }

        double t1, t2;
        double disc1, disc2;

        double t = m_dayCount->Time(m_baseDate, interpDate);
        if (lo < 0)
        {
            POST_CONDITION(hi == 0);
            t1 = 0.0;
            disc1 = 1.0;
            if (m_dates[0] > m_baseDate)
            {
                t2 = m_dayCount->Time(m_baseDate, m_dates[0]);
                disc2 = m_rateType.RateToDiscount(m_rates[0], t2);
            }
            else if (m_dates[0] == m_baseDate)
            {
                // in this case we must ignore [0] entry and use [1] instead
                if (m_dates.size() == 1)
                    THROW_RUNTIME_ERROR("Cannot extrapolate flat forwards with only one point at base date");

                t2 = m_dayCount->Time(m_baseDate, m_dates[1]);
                disc2 = m_rateType.RateToDiscount(m_rates[1], t2);
            }
            else
            {
                THROW_RUNTIME_ERROR("Cannot extrapolate before start of curve " 
                    << m_dates[0].ToString()
                    << " which is also before base date " << m_baseDate.ToString());
            }
        }
        else if (hi < 0)
        {
            POST_CONDITION(lo+1 == (int)m_rates.size());
            if (lo == 0)
            {
                if (m_dates[0] <= m_baseDate)
                    THROW_RUNTIME_ERROR("Cannot extrapolate flat forwards with only one point at base date");

                t1 = 0.0;
                disc1 = 1.0;
                t2 = m_dayCount->Time(m_baseDate, m_dates[0]);
                disc2 = m_rateType.RateToDiscount(m_rates[0], t2);
            }
            else
            {
                t1 = m_dayCount->Time(m_baseDate, m_dates[lo-1]);
                disc1 = m_rateType.RateToDiscount(m_rates[lo-1], t1);
                t2 = m_dayCount->Time(m_baseDate, m_dates[lo]);
                disc2 = m_rateType.RateToDiscount(m_rates[lo], t2);
            }
        }
        else
        {
            t1 = m_dayCount->Time(m_baseDate, m_dates[lo]);
            t2 = m_dayCount->Time(m_baseDate, m_dates[hi]);
            
            disc1 = m_rateType.RateToDiscount(m_rates[lo], t1);
            disc2 = m_rateType.RateToDiscount(m_rates[hi], t2);
        }

        POST_CONDITION(t2 >= t1);
        double discount;
        if (t2 > t1)
        {
            discount = disc1 * pow(disc2/disc1, (t-t1)/(t2-t1));
        }
        else
        {
            discount = sqrt(disc1 * disc2);
        }
        return discount;
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}


CZeroCurveProduct::CZeroCurveProduct(
    CDate baseDate,
    const IZeroCurveConstSP& zc1,
    const IZeroCurveConstSP& zc2,
    double baseDateDiscount)
    :
    m_baseDate(baseDate),
    m_zc1(zc1),
    m_zc2(zc2),
    m_baseDateDiscount(baseDateDiscount)
{
    VerifyAndComplete();
}

CDate CZeroCurveProduct::BaseDate() const
{
    return m_baseDate;
}

double CZeroCurveProduct::PV(CDate interpDate) const
{
    if (interpDate == m_baseDate)
        return 1.0;

    double pv1 = m_zc1->PV(interpDate);
    double pv2 = m_zc2->PV(interpDate);

    double pv = pv1 * pv2 / m_baseDateDiscount;

    return pv;
}

CDate CZeroCurveProduct::FirstDate() const
{
    return std::max(m_zc1->FirstDate(), m_zc2->FirstDate());
}

double CZeroCurveProduct::BaseDateDiscount() const
{
    return m_baseDateDiscount;
}

const IZeroCurveConstSP CZeroCurveProduct::ZC1() const
{
    return m_zc1;
}

const IZeroCurveConstSP CZeroCurveProduct::ZC2() const
{
    return m_zc2;
}

void CZeroCurveProduct::VerifyAndComplete()
{
    if (!m_zc1)
        SPI_UTIL_THROW_RUNTIME_ERROR("zc1 undefined");
    if (!m_zc2)
        SPI_UTIL_THROW_RUNTIME_ERROR("zc2 undefined");

    if (m_baseDateDiscount <= 0.0)
    {
        double baseDateDiscount = m_zc1->PV(m_baseDate) * m_zc2->PV(m_baseDate);
        m_baseDateDiscount = baseDateDiscount;
    }
}

double ForwardRate(
    const IZeroCurveConstSP& zeroCurve,
    CDate startDate,
    CDate maturityDate,
    const CDayCountConstSP& dayCount,
    CRateType rateType)
{
    try
    {
        double disc1 = zeroCurve->PV(startDate);
        double disc2 = zeroCurve->PV(maturityDate);
        double discount = disc2/disc1;

        return DiscountToRate(discount, startDate, maturityDate, dayCount, rateType);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

double FlowsPV(
    const CCashFlowListConstSP& cfl,
    const IZeroCurveConstSP& zc,
    CDate cutoffDate)
{
    PRE_CONDITION(cfl);
    PRE_CONDITION(zc);

    try
    {
        if (!cutoffDate)
            cutoffDate = zc->BaseDate();

        const std::vector<CDate>& dates = cfl->Dates();
        const std::vector<double>& amounts = cfl->Amounts();

        POST_CONDITION(dates.size() == amounts.size());

        size_t numItems = dates.size();

        double totalPV = 0.0;

        for (size_t i = 0; i < numItems; ++i)
        {
            const CDate& date = dates[i];
            if (date <= cutoffDate)
                continue;

            double discount = zc->PV(date);
            double pv = discount * amounts[i];

            totalPV += pv;
        }

        return totalPV;
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

double FlowsFV(
    const CCashFlowListConstSP& cfl,
    const IZeroCurveConstSP& zc,
    CDate valueDate,
    CDate cutoffDate)
{
    PRE_CONDITION(cfl);
    PRE_CONDITION(zc);
    PRE_CONDITION(valueDate);

    try
    {
        if (!cutoffDate)
            cutoffDate = valueDate;

        double pv = FlowsPV(cfl, zc, cutoffDate);
        double valueDatePV = zc->PV(valueDate);

        double fv = pv / valueDatePV;

        return fv;
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}
