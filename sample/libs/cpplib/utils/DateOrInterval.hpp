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
#ifndef CPPLIB_DATE_OR_INTERVAL_HPP
#define CPPLIB_DATE_OR_INTERVAL_HPP

/**
***************************************************************************
** DateOrInterval.hpp
***************************************************************************
** DateOrInterval class for defining maturity dates which can either be
** a specific date or arising from adding an interval to a date.
***************************************************************************
*/

#include "DateIntervalAdjust.hpp"

CPPLIB_DECLARE_CLASS(CDateOrInterval);
CPPLIB_DECLARE_CLASS(CDateOrIntervalDate);
CPPLIB_DECLARE_CLASS(CDateOrIntervalInterval);

class CDateOrInterval
{
public:
    virtual ~CDateOrInterval() {}
    virtual CDate EndDate(
        CDate startDate,
        const Holidays& holidays) const = 0;
};

class CDateOrIntervalDate : public CDateOrInterval
{
public:
    CDateOrIntervalDate(
        CDate date,
        EBadDayConv badDayConv);

    // implementation of CDateOrInterval interface
    CDate EndDate(
        CDate startDate,
        const Holidays& holidays) const;

    CDate Date() const;
    EBadDayConv BadDayConv() const;

private:

    CDate m_date;
    EBadDayConv m_badDayConv;
};

class CDateOrIntervalInterval : public CDateOrInterval
{
public:
    CDateOrIntervalInterval(
        const CDateInterval& interval,
        EBadDayConv badDayConv,
        EDateAdjType adjType);

    static CDateOrIntervalIntervalSP Make(
        const std::string& str);
    std::string ToString() const;

    // implementation of CDateOrInterval interface
    CDate EndDate(
        CDate startDate,
        const Holidays& holidays) const;

    const CDateInterval& Interval() const;
    EBadDayConv BadDayConv() const;
    EDateAdjType AdjType() const;

private:

    CDateInterval m_interval;
    EBadDayConv m_badDayConv;
    EDateAdjType m_adjType;
};

#endif


