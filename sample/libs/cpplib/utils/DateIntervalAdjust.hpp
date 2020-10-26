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
#ifndef CPPLIB_DATE_INTERVAL_ADJUST_HPP
#define CPPLIB_DATE_INTERVAL_ADJUST_HPP

/**
***************************************************************************
** DateIntervalAdjust.hpp
***************************************************************************
** Functions involving a mixture of date adjustment by DateInterval and
** Holidays.
***************************************************************************
*/

#include "Date.hpp"
#include "DateInterval.hpp"
#include "Holidays.hpp"

#include <string>

enum EDateAdjType : int
{
    CPPLIB_DATE_ADJ_TYPE_BUSINESS,
    CPPLIB_DATE_ADJ_TYPE_CALENDAR,
    CPPLIB_DATE_ADJ_TYPE_WEEKDAY
};

EDateAdjType DateAdjTypeFromString(const char* str);
EDateAdjType DateAdjTypeFromString(const std::string& str);
const std::string& DateAdjTypeToString(EDateAdjType adjType);

CDate DateFwdAdjust(
    CDate                startDate,
    const CDateInterval& interval,
    EDateAdjType         adjType,
    EBadDayConv          badDayConv,
    const Holidays&      holidays);

CDate DateFwdThenAdjust(
    CDate                startDate,
    const CDateInterval& interval,
    EBadDayConv          badDayConv,
    const Holidays&      holidays,
    int                  count=1);

#endif


