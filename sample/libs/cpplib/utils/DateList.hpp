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
#ifndef CPPLIB_DATELIST_HPP
#define CPPLIB_DATELIST_HPP

/**
***************************************************************************
** DateList.hpp
***************************************************************************
** DateList class containing a list of dates, plus various functions for
** generating and manipulating date lists.
***************************************************************************
*/

#include "Date.hpp"
#include "DateInterval.hpp"
#include "Declare.hpp"
#include <vector>

CPPLIB_DECLARE_CLASS(CDateList);

class CDateList
{
public:
    CDateList(const std::vector<CDate>& dates);

    int NumItems() const;
    const std::vector<CDate>& Dates() const;

    /**
    ************************************************************************
    ** Merges two date lists such that the resultant date list represents
    ** the union of the original lists (no duplicates). Resultant list
    ** will be in sorted order.
    ** 
    ** If either of the datelists is NULL, it just makes a copy of
    ** the other and returns it. If both are NULL, it returns NULL.
    ************************************************************************
    */
    static CDateListSP Merge(
        const CDateListConstSP& dateList1,
        const CDateListConstSP& dateList2);

    /**
    ************************************************************************
    ** Generates date list from startDate, endDate and date interval.
    ************************************************************************
    */
    static CDateListSP Generate(
        CDate startDate,
        CDate endDate,
        const CDateInterval& interval,
        bool stubAtEnd,
        bool longStub=false);

private:
    std::vector<CDate> m_dates;
};

#endif


