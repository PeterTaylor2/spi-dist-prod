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
/*
***************************************************************************
** DateList.cpp
***************************************************************************
** Implementation of the CDateList class.
***************************************************************************
*/

#include "DateList.hpp"

#include "RuntimeError.hpp"

#include <spi_util/StringUtil.hpp>
#include <spi_util/DateUtil.hpp>

#include <set>

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_ANONYMOUS_NAMESPACE

CDateList* NewDateList(const std::set<CDate>& setOfDates)
{
    std::vector<CDate> dates;
    for (std::set<CDate>::const_iterator iter = setOfDates.begin();
        iter != setOfDates.end(); ++iter)
    {
        dates.push_back(*iter);
    }

    return new CDateList(dates);
}

END_ANONYMOUS_NAMESPACE

CDateList::CDateList(const std::vector<CDate>& dates)
:
m_dates(dates)
{}

int CDateList::NumItems() const
{
    return (int)m_dates.size();
}

const std::vector<CDate>& CDateList::Dates() const
{
    return m_dates;
}

/*
***************************************************************************
** Merges two date lists such that the resultant date list represents
** the union of the original lists (no duplicates). Resultant list
** will be in sorted order.
** 
** If either of the datelists is NULL, it just makes a copy of
** the other and returns it. If both are NULL, it returns NULL.
***************************************************************************
*/
CDateListSP CDateList::Merge(
    const CDateListConstSP& dateList1,
    const CDateListConstSP& dateList2)
{
    if (!dateList1 && !dateList2)
    {
        return CDateListSP();
    }
    if (!dateList1)
    {
        return CDateListSP(new CDateList(dateList2->Dates()));
    }
    if (!dateList2)
    {
        return CDateListSP(new CDateList(dateList1->Dates()));
    }

    std::set<CDate> dates;
    const std::vector<CDate> dates1 = dateList1->Dates();
    const std::vector<CDate> dates2 = dateList2->Dates();

    size_t size = dates1.size();
    for (size_t i = 0; i < size; ++i)
        dates.insert(dates1[i]);

    size = dates2.size();
    for (size_t i = 0; i < size; ++i)
        dates.insert(dates2[i]);

    return CDateListSP(NewDateList(dates));
}

/**
************************************************************************
** Generates date list from startDate, endDate and date interval.
************************************************************************
*/
CDateListSP CDateList::Generate(
    CDate startDate,
    CDate endDate,
    const CDateInterval& interval,
    bool stubAtEnd,
    bool longStub)
{
    if (endDate < startDate)
    {
        THROW_RUNTIME_ERROR("EndDate " << endDate.ToString() 
            << " is before startDate " << startDate.ToString());
    }

    std::set<CDate> dates;
    if (interval.Years() <= 0.0)
    {
        THROW_RUNTIME_ERROR("DateInterval '" << interval.ToString()
            << "' should be positive.");
    }

    dates.insert(startDate);
    dates.insert(endDate);

    bool hasStub = false;
    CDate lastDateInserted;

    if (stubAtEnd)
    {
        int count = 1;
        for (;;)
        {
            CDate date = interval.AddToDate(endDate, -count);
            POST_CONDITION(date < endDate);
            if (date <= startDate)
            {
                hasStub = date < startDate;
                break;
            }
            dates.insert(date);
            lastDateInserted = date;
            ++count;
        }
    }
    else
    {
        int count = 1;
        for (;;)
        {
            CDate date = interval.AddToDate(startDate, count);
            POST_CONDITION(date > startDate);
            if (date >= endDate)
            {
                hasStub = date > endDate;
                break;
            }
            dates.insert(date);
            lastDateInserted = date;
            ++count;
        }
    }

    if (hasStub && longStub && lastDateInserted)
    {
        dates.erase(lastDateInserted);
    }

    return CDateListSP(NewDateList(dates));
}

