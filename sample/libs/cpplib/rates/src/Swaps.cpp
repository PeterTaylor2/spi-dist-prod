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
** Swaps.hpp
***************************************************************************
** Swaps analytics.
***************************************************************************
*/

#include "Swaps.hpp"

#include "ZeroCurve.hpp"
#include "CashFlowList.hpp"
#include <utils/DateList.hpp>

#include <utils/EnumLookup.hpp>
#include <utils/RuntimeError.hpp>
#include <spi_util/Utils.hpp>

#include <math.h>
#include <string.h>

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_ANONYMOUS_NAMESPACE


END_ANONYMOUS_NAMESPACE

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
    const Holidays&         holidays)
{
    try
    {
        if (maturityDate <= startDate)
        {
            THROW_RUNTIME_ERROR("maturityDate(" << maturityDate.ToString()
                << " <= startDate(" << startDate.ToString());
        }

        CDateListSP dl = CDateList::Generate(
            startDate, maturityDate, couponInterval, !frontStub, !shortStub);

        std::vector<CDate> dates;
        std::vector<double> amounts;

        int numDates = dl->NumItems();
        const std::vector<CDate>& dateList = dl->Dates();

        // we always get startDate and maturityDate within dateList
        // stubs are placed according to the request

        if (subtractInitial)
        {
            dates.push_back(BusinessDay(dateList[0], payBadDayConv, holidays));
            amounts.push_back(-1);
        }
        else if (keepStartDate)
        {
            dates.push_back(BusinessDay(dateList[0], payBadDayConv, holidays));
            amounts.push_back(0);
        }

        for (int i = 1; i < numDates; ++i)
        {
            const CDate& priorDate  = dateList[i-1];
            const CDate& couponDate = dateList[i];

            double time = dayCount->Time(
                BusinessDay(priorDate, accBadDayConv, holidays),
                BusinessDay(couponDate, accBadDayConv, holidays));

            double amount = time * couponRate;

            dates.push_back(BusinessDay(couponDate, payBadDayConv, holidays));
            amounts.push_back(amount);
        }

        if (addFinal)
        {
            amounts[amounts.size()-1] += 1;
        }

        return Share(new CCashFlowList(dates, amounts));
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

double SwapRate(
    const IZeroCurveConstSP& zc,
    CDate startDate,
    const CDateInterval& couponInterval,
    CDate maturityDate,
    const CDayCountConstSP& dayCount,
    EBadDayConv accBadDayConv,
    EBadDayConv payBadDayConv,
    const Holidays& holidays,
    bool stubAtEnd)
{
    try
    {
        CDate startDateAdj = BusinessDay(startDate, accBadDayConv, holidays);
        CDate zcFirstDate  = zc->FirstDate();

        if (startDateAdj < zcFirstDate)
        {
            THROW_RUNTIME_ERROR("Adjusted start date (" << startDateAdj.ToString()
                << ") is before first valid curve date " << zcFirstDate.ToString());
        }

        bool shortStub = true;
        bool frontStub = !stubAtEnd;
        bool subtractInitial = false;
        bool keepStartDate = false;
        bool addFinal = false;

        CCashFlowListConstSP annuity = SwapFixedFlows(1.0, startDate,
            couponInterval, maturityDate, dayCount,
            frontStub, shortStub, subtractInitial, keepStartDate, addFinal,
            accBadDayConv, payBadDayConv, holidays);

        double startDatePV = zc->PV(startDateAdj);
        double finalDatePV = zc->PV(annuity->Dates().back());
        double annuityPV = FlowsPV(annuity, zc, startDateAdj);

        double couponRate = (startDatePV - finalDatePV) / annuityPV;

        return couponRate;
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

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
    bool                    stubAtEnd)
{
    try
    {
        double swapRate = SwapRate(zc, startDate, couponInterval,
            maturityDate, dayCount, accBadDayConv, payBadDayConv,
            holidays, stubAtEnd);

        if (getParFlows)
            couponRate = swapRate;

        CCashFlowListConstSP cfl = SwapFixedFlows(
            couponRate, startDate, couponInterval, maturityDate,
            dayCount, !stubAtEnd, false, false, false, true,
            accBadDayConv, payBadDayConv, holidays);

        CMapSP results(new CMap);
        results->Insert("swapRate", swapRate);
        results->Insert("fixedFlows", cfl);

        CCashFlowListConstSP justChecking = results->Item("fixedFlows");

        return results;
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}
