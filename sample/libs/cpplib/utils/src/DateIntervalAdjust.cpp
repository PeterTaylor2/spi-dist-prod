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
** DateIntervalAdjust.hpp
***************************************************************************
** Functions involving a mixture of date adjustment by DateInterval and
** Holidays.
***************************************************************************
*/

#include "DateIntervalAdjust.hpp"

#include "EnumLookup.hpp"
#include "RuntimeError.hpp"
#include <spi_util/StringUtil.hpp>

#include <ctype.h>
#include <stdlib.h>

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_ANONYMOUS_NAMESPACE

CEnumLookup* DateAdjTypeEnumLookup()
{
    static CEnumLookup lookup("DateAdjType");
    static bool init = false;

    if (!init)
    {
        lookup.AddItem((int)CPPLIB_DATE_ADJ_TYPE_BUSINESS, "Business");
        lookup.AddItem((int)CPPLIB_DATE_ADJ_TYPE_BUSINESS, "B");
        lookup.AddItem((int)CPPLIB_DATE_ADJ_TYPE_CALENDAR, "Calendar");
        lookup.AddItem((int)CPPLIB_DATE_ADJ_TYPE_CALENDAR, "C");
        lookup.AddItem((int)CPPLIB_DATE_ADJ_TYPE_WEEKDAY, "Weekday");
        lookup.AddItem((int)CPPLIB_DATE_ADJ_TYPE_WEEKDAY, "W");
        init = true;
    }

    return &lookup;
}

END_ANONYMOUS_NAMESPACE

EDateAdjType DateAdjTypeFromString(const char* str)
{
    int code = DateAdjTypeEnumLookup()->FromString(str);
    return (EDateAdjType)code;
}

const std::string& DateAdjTypeToString(
    EDateAdjType adjType)
{
    return DateAdjTypeEnumLookup()->ToString((int)adjType);
}

/*
**************************************************************************
** Implementation of stand-alone functions
**************************************************************************
*/
CDate DateFwdAdjust(
    CDate                startDate,
    const CDateInterval& interval,
    EDateAdjType         adjType,
    EBadDayConv          badDayConv,
    const Holidays&      holidays)
{
    try
    {
        switch(adjType)
        {
        case CPPLIB_DATE_ADJ_TYPE_BUSINESS:
            if (interval.IsDaily())
            {
                int numDays = interval.NumDays();
                return AddBusinessDays(startDate, numDays, holidays);
            }
            else
            {
                /*
                ** Non-daily interval and offset by business days.
                ** This means that if the start date is the end of the business
                ** month, then the end date must also be the end of the month.
                ** Actually we do adjust the end date by the bad day convention,
                ** but in the context of generating a set of cash flow dates,
                ** this is often not done when computing the maturity date.
                */
                CDate businessEOM = BusinessEndOfMonth(startDate, holidays);
                CDate endDate = interval.AddToDate(startDate);
                if (startDate == businessEOM)
                {
                    endDate = BusinessEndOfMonth(endDate, holidays);
                }
                return BusinessDay(endDate, badDayConv, holidays);
            }
            break;
        case CPPLIB_DATE_ADJ_TYPE_CALENDAR:
            return DateFwdThenAdjust(
                startDate, interval, badDayConv, holidays);
        case CPPLIB_DATE_ADJ_TYPE_WEEKDAY:
            /* Adjust interval as weekdays, and then bad day adjust the result. */
            if (interval.IsDaily())
            {
                CDate endDate = AddBusinessDays(
                    startDate, interval.NumDays(), "NONE");
                return BusinessDay(endDate, badDayConv, holidays);
            }
            THROW_RUNTIME_ERROR("Weekday adjustment requires date interval "
                << interval.ToString() << " to be in days");
            break;
        default:
            THROW_RUNTIME_ERROR("Unsupported date adjustment type: " << (int)adjType);
        }
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

CDate DateFwdThenAdjust(
    CDate                startDate,
    const CDateInterval& interval,
    EBadDayConv          badDayConv,
    const Holidays&      holidays,
    int                  count)
{
    try
    {
        CDate endDate = interval.AddToDate(startDate, count);
        return BusinessDay(endDate, badDayConv, holidays);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

