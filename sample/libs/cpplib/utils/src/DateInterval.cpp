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
** DateInterval.cpp
***************************************************************************
** DateInterval class for adjusting dates.
***************************************************************************
*/

#include "DateInterval.hpp"

#include "EnumLookup.hpp"
#include "RuntimeError.hpp"

#include <ctype.h>
#include <stdlib.h>

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_ANONYMOUS_NAMESPACE

CEnumLookup* DateIntervalPeriodTypeEnumLookup()
{
    static CEnumLookup lookup("DateInterval::PeriodType");
    static bool init = false;

    if (!init)
    {
        lookup.AddItem((int)CDateInterval::DAY, "D");
        lookup.AddItem((int)CDateInterval::DAY, "");
        lookup.AddItem((int)CDateInterval::WEEK, "W");
        lookup.AddItem((int)CDateInterval::MONTH, "M");
        lookup.AddItem((int)CDateInterval::YEAR, "Y");
        lookup.AddItem((int)CDateInterval::IMM_MONTH, "J");
        lookup.AddItem((int)CDateInterval::IMM_QUARTER, "I");
        lookup.AddItem((int)CDateInterval::MONTH_EOM, "M,EOM");
        init = true;
    }

    return &lookup;
}

CDateInterval::EPeriodType DateIntervalPeriodTypeFromString(const char* str)
{
    int code = DateIntervalPeriodTypeEnumLookup()->FromString(str);
    return (CDateInterval::EPeriodType)code;
}

const std::string& DateIntervalPeriodTypeToString(
    CDateInterval::EPeriodType periodType)
{
    return DateIntervalPeriodTypeEnumLookup()->ToString((int)periodType);
}

END_ANONYMOUS_NAMESPACE

CDate AddMonths(CDate startDate, int numMonths, bool eom)
{
    if (numMonths == 0)
        return startDate;

    int year;
    int month;
    int day;

    startDate.YMD(&year, &month, &day);

    int newYear  = year;
    int newMonth = month + numMonths;
    int newDay   = day;

    if (newMonth > 12)
    {
        int addYears = (newMonth-1) / 12;
        newYear += addYears;
        newMonth -= (12 * addYears);
        POST_CONDITION(newMonth >= 1 && newMonth <= 12);
    }
    else if (newMonth <= 0)
    {
        int subtractYears = (12-newMonth) / 12;
        newYear -= subtractYears;
        newMonth += (12 * subtractYears);
        POST_CONDITION(newMonth >= 1 && newMonth <= 12);
    }

    if (eom && day >= 28)
    {
        int lastDay = LastDayInMonth(year, month);
        if (day == lastDay)
            newDay = LastDayInMonth(newYear, newMonth);
    }
    else if (newDay > 28)
    {
        int lastDay = LastDayInMonth(newYear, newMonth);
        if (newDay > lastDay)
            newDay = lastDay;
    }

    return CDate(newYear, newMonth, newDay);
}

CDateInterval::CDateInterval()
    :
    m_numPeriods(0),
    m_periodType(DAY)
{}

CDateInterval::CDateInterval(
    int  numPeriods,
    EPeriodType periodType)
    :
    m_numPeriods(numPeriods),
    m_periodType(periodType)
{
    VerifyAndComplete();
}

void CDateInterval::VerifyAndComplete()
{}

CDateInterval::CDateInterval(const std::string& str)
{
    char* endPtr;
    long numPeriods = strtol(str.c_str(), &endPtr, 10);

    EPeriodType periodType = DateIntervalPeriodTypeFromString(endPtr);

    m_numPeriods = numPeriods;
    m_periodType = periodType;
}

std::string CDateInterval::ToString() const
{    
    std::ostringstream oss;

    oss << m_numPeriods << DateIntervalPeriodTypeToString(m_periodType);

    return oss.str();
}

CDateInterval::operator std::string() const
{
    return ToString();
}

CDateInterval::operator bool() const
{
    return m_numPeriods != 0;
}

bool CDateInterval::IsDaily() const
{
    switch(m_periodType)
    {
    case DAY:
    case WEEK:
        return true;
    case MONTH:
    case YEAR:
    case IMM_MONTH:
    case IMM_QUARTER:
    case MONTH_EOM:
        return false;
    }

    THROW_RUNTIME_ERROR("Bad period type " << (int)m_periodType);
}

int CDateInterval::NumDays() const
{
    switch(m_periodType)
    {
    case DAY:
        return m_numPeriods;
    case WEEK:
        return 7 * m_numPeriods;
    default:
        THROW_RUNTIME_ERROR("DateInterval is not a daily period");
    }
}

bool CDateInterval::IsIMM() const
{
    switch(m_periodType)
    {
    case DAY:
    case WEEK:
    case MONTH:
    case YEAR:
    case MONTH_EOM:
        return false;
    case IMM_MONTH:
    case IMM_QUARTER:
        return true;
    }

    THROW_RUNTIME_ERROR("Bad period type " << (int)m_periodType);
}

bool CDateInterval::IsMonthly() const
{
    switch(m_periodType)
    {
    case DAY:
    case WEEK:
        return false;
    case MONTH:
    case YEAR:
    case MONTH_EOM:
        return true;
    case IMM_MONTH:
    case IMM_QUARTER:
        return false;
    }

    THROW_RUNTIME_ERROR("Bad period type " << (int)m_periodType);
}

int CDateInterval::NumMonths() const
{
    switch(m_periodType)
    {
    case MONTH_EOM:
    case MONTH:
        return m_numPeriods;
    case YEAR:
        return 12 * m_numPeriods;
    default:
        THROW_RUNTIME_ERROR("DateInterval is not a monthly period");
    }
}

double CDateInterval::Years() const
{
    switch(m_periodType)
    {
    case DAY:
        return m_numPeriods / 365.0;
    case WEEK:
        return 7 * m_numPeriods / 365.0;
    case MONTH:
    case MONTH_EOM:
    case IMM_MONTH:
        return m_numPeriods / 12.0;
    case YEAR:
        return m_numPeriods;
    case IMM_QUARTER:
        return m_numPeriods / 4.0;
    }

    THROW_RUNTIME_ERROR("Bad period type " << (int)m_periodType);
}

double CDateInterval::Frequency() const
{
    double years = Years();
    if (years > 0.0)
        return 1.0 / years;

    THROW_RUNTIME_ERROR("Interval '" << ToString() 
        << "' is zero or negative");
}

int CDateInterval::NumPeriods() const
{
    return m_numPeriods;
}

CDateInterval::EPeriodType CDateInterval::PeriodType() const
{
    return m_periodType;
}

CDate CDateInterval::AddToDate(CDate startDate, int count) const
{
    int numIntervals = count * m_numPeriods;
    if (numIntervals == 0)
        return startDate;

    switch(m_periodType)
    {
    case DAY:
        return startDate + numIntervals;
    case WEEK:
        return startDate + (7*numIntervals);
    case MONTH:
        return AddMonths(startDate, numIntervals, false);
    case YEAR:
        return AddMonths(startDate, 12*numIntervals, false);
    case MONTH_EOM:
        return AddMonths(startDate, numIntervals, true);
    case IMM_MONTH:
    case IMM_QUARTER:
        NOT_IMPLEMENTED;
    }

    THROW_RUNTIME_ERROR("Bad period type " << (int)m_periodType);
}

std::vector<CDate> GenerateDates(
    CDate startDate,
    CDate endDate,
    const CDateInterval& interval,
    bool stubAtEnd)
{
    NOT_IMPLEMENTED;
}

CDate operator+ (CDate startDate, const CDateInterval& ivl)
{
    return ivl.AddToDate(startDate);
}

CDate operator- (CDate startDate, const CDateInterval& ivl)
{
    return ivl.AddToDate(startDate, -1);
}
