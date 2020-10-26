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

#include "ZCBuild.hpp"

#include "Swaps.hpp"

#include <utils/RuntimeError.hpp>
#include <utils/RootBrent.hpp>
#include <float.h>

#include <spi_util/ClockUtil.hpp>

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_ANONYMOUS_NAMESPACE

class ZCSwapsBuilder;

class SwapRateFunction : public CFunction1
{
public:
    SwapRateFunction(ZCSwapsBuilder* builder) : m_builder(builder)
    {}

    double operator()(double x) const;

private:
    ZCSwapsBuilder* m_builder;
};


class ZCSwapsBuilder
{
public:
    ZCSwapsBuilder(
        const CZeroCurveConstSP& stubCurve,
        const CDateInterval&     couponInterval,
        const CDayCountConstSP&  dayCountConv,
        EBadDayConv              badDayConv,
        const Holidays&          holidays)
        :
        m_baseDate(stubCurve->BaseDate()),
        m_interpType(stubCurve->InterpType()),
        m_dates(stubCurve->Dates()),
        m_rates(stubCurve->Rates()),
        m_rateType(stubCurve->RateType()),
        m_dayCount(stubCurve->DayCount()),
        m_couponInterval(couponInterval),
        m_swapDayCount(dayCountConv),
        m_badDayConv(badDayConv),
        m_holidays(holidays),
        m_maturityDate(),
        m_pos()
    {}

    double SwapRate() const
    {
        return ::SwapRate(
            ZeroCurve(),
            m_baseDate,
            m_couponInterval,
            m_maturityDate,
            m_swapDayCount,
            m_badDayConv,
            m_badDayConv,
            m_holidays);
    }

    void SetLastRate(double lastZeroRate)
    {
        m_rates[m_pos] = lastZeroRate;
    }

    void AddSwapRate(CDate date, double rate)
    {
        try
        {

            PRE_CONDITION(date > LastDate());

            m_maturityDate = date;
            m_dates.push_back(BusinessDay(date, m_badDayConv, m_holidays));
            m_rates.push_back(rate);

            POST_CONDITION(m_dates.size() == m_rates.size());
            POST_CONDITION(m_dates.size() >= 1);
            m_pos = m_dates.size() - 1;

            SwapRateFunction func(this);
            double lastZeroRate = RootFindBrent(
                func,
                rate,
                -1.0,
                10.0,
                100,
                rate,
                DBL_MAX,
                1e-10);

            SetLastRate(lastZeroRate);
        }
        catch (std::exception& e)
        {
            RETHROW(e, "Could not add swap with maturity " << date.ToString()
                << " and rate " << rate);
        }
    }

    CZeroCurveConstSP ZeroCurve() const
    {
        return ShareConst(new CZeroCurve(
            m_baseDate, m_interpType, m_dates, m_rates,
            m_rateType, m_dayCount));
    }

private:

    CDate                   m_baseDate;
    CZeroCurve::EInterpType m_interpType;
    std::vector<CDate>      m_dates;
    std::vector<double>     m_rates;
    CRateType               m_rateType;
    CDayCountConstSP        m_dayCount;

    CDateInterval           m_couponInterval;
    CDayCountConstSP        m_swapDayCount;
    EBadDayConv             m_badDayConv;
    Holidays                m_holidays;

    CDate                   m_maturityDate;
    size_t                  m_pos;

    CDate LastDate() const
    {
        if (m_dates.size() == 0)
            return m_baseDate;

        return std::max(m_dates.back(), m_baseDate);
    }

};
        
double SwapRateFunction::operator()(double lastZeroRate) const
{
    m_builder->SetLastRate(lastZeroRate);
    return m_builder->SwapRate();
}


END_ANONYMOUS_NAMESPACE


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
    CZeroCurve::EInterpType    interpType)
{
    try
    {
        PRE_CONDITION(dayCount);
        PRE_CONDITION(baseDate);
        CheckArraySize(dates, rates, "cashDates", "cashRates");

        SPI_UTIL_CLOCK_EVENTS_START();

        std::map<CDate, double> allRates;
        size_t nbDates = dates.size();

        for (size_t i = 0; i < nbDates; ++i)
        {
            const CDate& date = dates[i];
            if (allRates.count(date) > 0)
                THROW_RUNTIME_ERROR("Duplicate date " << date.ToString());
            if (date <= baseDate)
                THROW_RUNTIME_ERROR("Date " << date.ToString() 
                    << " is on or before baseDate " << baseDate.ToString());
            allRates[date] = rates[i];
        }

        SPI_UTIL_CLOCK_EVENTS_LOG("InputsSorted");
        std::vector<CDate> zcDates;
        std::vector<double> zcRates;

        const CRateType SIMPLE(CRateType::SIMPLE);
        const CRateType ANNUAL(CRateType::ANNUAL);

        for (std::map<CDate,double>::const_iterator iter = allRates.begin();
             iter != allRates.end(); ++iter)
        {
            const CDate&  date = iter->first;
            const double& rate = iter->second;

            double discount = RateToDiscount(rate, baseDate, date, 
                dayCount, SIMPLE);
            double zcRate = DiscountToRate(discount, baseDate, date,
                CDayCount::ACT_365F, ANNUAL);

            zcDates.push_back(date);
            zcRates.push_back(zcRate);
            SPI_UTIL_CLOCK_EVENTS_LOG("RateAdded");
        }

        SPI_UTIL_CLOCK_EVENTS_WRITE("ZeroCurveCash.log");
        return ShareConst(new CZeroCurve(baseDate, interpType, 
            zcDates, zcRates, ANNUAL, CDayCount::ACT_365F));
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

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
    const Holidays&            holidays)
{
    try
    {
        PRE_CONDITION(stubCurve);
        PRE_CONDITION(dayCountConv);

        SPI_UTIL_CLOCK_EVENTS_START();

        CheckArraySize(dates, rates, "swapDates", "swapRates");
        size_t numSwaps = dates.size();
        CDate baseDate = stubCurve->BaseDate();
        CDate lastStubDate = stubCurve->LastDate();

        if (!IsBusinessDay(baseDate, holidays))
            THROW_RUNTIME_ERROR("Base date of stub curve " 
                << baseDate.ToString()
                << " is not a business day");

        std::map<CDate, double> allRates;

        for (size_t i = 0; i < numSwaps; ++i)
        {
            const CDate& date = dates[i];
            if (allRates.count(date) > 0)
                THROW_RUNTIME_ERROR("Duplicate date " << date.ToString());
            if (date <= baseDate)
                THROW_RUNTIME_ERROR("Date " << date.ToString() 
                    << " is on or before baseDate " << baseDate.ToString());
            if (date <= lastStubDate)
                continue; // ignore
            allRates[date] = rates[i];
        }

        SPI_UTIL_CLOCK_EVENTS_LOG("InputsSorted");

        ZCSwapsBuilder builder(stubCurve, couponInterval, dayCountConv,
            badDayConv, holidays);

        for (std::map<CDate,double>::const_iterator iter = allRates.begin();
             iter != allRates.end(); ++iter)
        {
            const CDate&  date = iter->first;
            const double& rate = iter->second;

            builder.AddSwapRate(date, rate);

            SPI_UTIL_CLOCK_EVENTS_LOG("RateAdded");
        }

        SPI_UTIL_CLOCK_EVENTS_WRITE("ZeroCurveSwaps.log");
        return builder.ZeroCurve();
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

