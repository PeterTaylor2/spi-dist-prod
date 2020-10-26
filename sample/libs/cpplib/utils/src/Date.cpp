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
** Date.cpp
***************************************************************************
** Implementation of the CDate class.
***************************************************************************
*/

#include "Date.hpp"

// #include "Format.hpp"
#include "RuntimeError.hpp"

#include <spi_util/StringUtil.hpp>
#include <spi_util/DateUtil.hpp>

#include <time.h>

const char* WeekdayName(EWeekday weekday)
{
    switch (weekday)
    {
    case MONDAY: return "Monday";
    case TUESDAY: return "Tuesday";
    case WEDNESDAY: return "Wednesday";
    case THURSDAY: return "Thursday";
    case FRIDAY: return "Friday";
    case SATURDAY: return "Saturday";
    case SUNDAY: return "Sunday";
    }

    THROW_RUNTIME_ERROR("Unknown weekday " << (int)weekday);
}

CDate::CDate() : m_dt(0)
{}

CDate::CDate(int dt) : m_dt(dt)
{
    PRE_CONDITION(dt >= 0);
}

CDate::CDate(int year, int month, int day) : m_dt(0)
{
    PRE_CONDITION(year > 1600);
    PRE_CONDITION(month >= 1 && month <= 12);
    PRE_CONDITION(day >= 1 && day <= 31);

    m_dt = spi_util::YMDToDate(year, month, day);
}

CDate CDate::FromString(const std::string& str)
{
    return CDate(spi_util::StringToDate(str));
}

CDate CDate::Today()
{
    time_t t = time(0);

#ifdef _MSC_VER
    struct tm* now = localtime(&t);
    return CDate(now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
#else
    struct tm now;
    localtime_r(&t, &now);
    return CDate(now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);
#endif
}

void CDate::YMD(int* year, int* month, int* day) const
{
    spi_util::DateToYMD(m_dt, year, month, day);
}

int CDate::Year() const
{
    int year;
    spi_util::DateToYMD(m_dt, &year, NULL, NULL);
    return year;
}

int CDate::Month() const
{
    int month;
    spi_util::DateToYMD(m_dt, NULL, &month, NULL);
    return month;
}

int CDate::Day() const
{
    int day;
    spi_util::DateToYMD(m_dt, NULL, NULL, &day);
    return day;
}

EWeekday CDate::Weekday() const
{
    const int magic = 0;
    int weekday = (m_dt + magic) % 7;
    return (EWeekday)weekday;
}

CDate CDate::EndOfMonth() const
{
    int year, month, day;
    YMD(&year, &month, &day);
    return CDate(year, month, LastDayInMonth(year, month));
}

CDate CDate::StartOfMonth() const
{
    int year, month, day;
    YMD(&year, &month, &day);
    return CDate(year, month, 1);
}

CDate::operator int() const
{
    return m_dt;
}

CDate::operator std::string() const
{
    return spi_util::DateToString(m_dt);
}

std::string CDate::ToString() const
{
    return spi_util::DateToString(m_dt);
}

CDate& CDate::operator += (int days)
{
    int dt = m_dt + days;
    if (dt < 0)
    {
        THROW_RUNTIME_ERROR("Adding " << days << " days to " << ToString()
                            << " causes date overflow");
    }
    m_dt = dt;
    return *this;
}

CDate& CDate::operator -= (int days)
{
    int dt = m_dt - days;
    if (dt < 0)
    {
        THROW_RUNTIME_ERROR("Subtracting " << days << " days from " 
                            << ToString() << " causes date overflow");
    }
    m_dt = dt;
    return *this;
}

bool IsLeapYear(int year)
{
    if (year < 1601)
        THROW_RUNTIME_ERROR("Year " << year << " should be at least 1601");
    if (year % 4 != 0)
        return false;
    if (year % 100 != 0)
        return true;
    if (year % 400 == 0)
        return true;
    return false;
}

int LastDayInMonth(int year, int month)
{
    return spi_util::DaysInMonth(year, month);
}
        
CDate NthWeekdayInMonth(int year, int month, int n, EWeekday weekday)
{
    PRE_CONDITION(n >= 1);
    PRE_CONDITION(n <= 5);

    EWeekday firstWeekday = CDate(year, month, 1).Weekday();
    
    int diffDay = (int)weekday - (int)firstWeekday;
    if (diffDay < 0)
        diffDay += 7;

    POST_CONDITION(diffDay >= 0);
    int firstDay = diffDay + 1;
    int nthWeekDay = firstDay + (n-1) * 7;

    try
    {
        return CDate(year, month, nthWeekDay);
    }
    catch (...)
    {
        THROW_RUNTIME_ERROR("Year " << year << " Month " << month 
            << " does not have " << n << " " << WeekdayName(weekday) << "s");
    }
}

