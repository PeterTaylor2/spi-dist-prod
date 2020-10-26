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
** DateOrInterval.cpp
***************************************************************************
** DateOrInterval class for defining maturity dates which can either be
** a specific date or arising from adding an interval to a date.
***************************************************************************
*/

#include "DateOrInterval.hpp"

#include "EnumLookup.hpp"
#include "RuntimeError.hpp"
#include <spi_util/StringUtil.hpp>

#include <ctype.h>
#include <stdlib.h>

/*
*****************************************************************************
** Implementation of CDateOrIntervalDate
*****************************************************************************
*/
CDateOrIntervalDate::CDateOrIntervalDate(
    CDate date,
    EBadDayConv badDayConv)
    :
    m_date(date),
    m_badDayConv(badDayConv)
{}

CDate CDateOrIntervalDate::EndDate(
    CDate startDate,
    const Holidays& holidays) const
{
    try
    {
        return BusinessDay(m_date, m_badDayConv, holidays);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

CDate CDateOrIntervalDate::Date() const
{
    return m_date;
}

EBadDayConv CDateOrIntervalDate::BadDayConv() const
{
    return m_badDayConv;
}

/*
*****************************************************************************
** Implementation of CDateOrIntervalInterval
*****************************************************************************
*/
CDateOrIntervalInterval::CDateOrIntervalInterval(
    const CDateInterval& interval,
    EBadDayConv badDayConv,
    EDateAdjType adjType)
    :
    m_interval(interval),
    m_badDayConv(badDayConv),
    m_adjType(adjType)
{}

CDateOrIntervalIntervalSP CDateOrIntervalInterval::Make(
    const std::string& str)
{
    PRE_CONDITION(str.length() > 0);

    try
    {
        std::vector<std::string> parts = spi_util::StringSplit(str, ',');
        POST_CONDITION(parts.size() > 0);

        CDateInterval interval(parts[0]); // initialized from string
        EBadDayConv   badDayConv = CPPLIB_BAD_DAY_NONE;
        EDateAdjType  adjType    = CPPLIB_DATE_ADJ_TYPE_CALENDAR;

        switch(parts.size())
        {
        case 1: // e.g. 3M
            break;
        case 2:
            if (parts[1].length() != 1)
                THROW_RUNTIME_ERROR("Second part of string must be a single character");

            switch(toupper(parts[1][0]))
            {
            case 'M':
            case 'F':
            case 'N':
            case 'P':
                badDayConv = BadDayConvFromString(parts[1]);
                break;
            case 'W':
                adjType = CPPLIB_DATE_ADJ_TYPE_WEEKDAY;
                break;
            case 'B':
                adjType = CPPLIB_DATE_ADJ_TYPE_BUSINESS;
                break;
            case 'C':
                adjType = CPPLIB_DATE_ADJ_TYPE_CALENDAR;
                break;
            default:
                THROW_RUNTIME_ERROR("Second part of string must be M,F,N,P,W,B or C");
            }
            break;
        case 3: // e.g. 2D,W,F
            if (parts[1].length() != 1 || parts[2].length() != 1)
                THROW_RUNTIME_ERROR("Second and third parts of string must be a single character");
            adjType    = DateAdjTypeFromString(parts[1].c_str());
            badDayConv = BadDayConvFromString(parts[2]);
            break;
        default:
            THROW_RUNTIME_ERROR("Too many parts in string");
        }
        return Share(new CDateOrIntervalInterval(interval, badDayConv, adjType));
    }
    catch (std::exception& e)
    {
        RETHROW(e, "Cannot convert '" << str << "' to DateOrIntervalInterval");
    }
}

std::string CDateOrIntervalInterval::ToString() const
{
    std::ostringstream oss;

    oss << m_interval.ToString();

    if (m_adjType != CPPLIB_DATE_ADJ_TYPE_CALENDAR)
    {
        oss << "," << DateAdjTypeToString(m_adjType)[0];
    }

    if (m_badDayConv != CPPLIB_BAD_DAY_NONE)
    {
        oss << "," << BadDayConvToString(m_badDayConv)[0];
    }

    return oss.str();
}

CDate CDateOrIntervalInterval::EndDate(
    CDate startDate,
    const Holidays& holidays) const
{
    try
    {
        return DateFwdAdjust(
            startDate,
            m_interval,
            m_adjType,
            m_badDayConv,
            holidays);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

EDateAdjType CDateOrIntervalInterval::AdjType() const
{
    return m_adjType;
}

EBadDayConv CDateOrIntervalInterval::BadDayConv() const
{
    return m_badDayConv;
}

const CDateInterval& CDateOrIntervalInterval::Interval() const
{
    return m_interval;
}

