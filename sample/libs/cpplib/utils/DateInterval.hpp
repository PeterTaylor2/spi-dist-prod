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
#ifndef CPPLIB_DATE_INTERVAL_HPP
#define CPPLIB_DATE_INTERVAL_HPP

/**
***************************************************************************
** DateInterval.hpp
***************************************************************************
** DateInterval class for adjusting dates.
***************************************************************************
*/

#include "Date.hpp"
#include "Declare.hpp"

#include <string>
#include <vector>

CDate AddMonths(CDate startDate, int numMonths, bool eom);

class CDateInterval
{
public:

    enum EPeriodType
    {
        DAY,
        WEEK,
        MONTH,
        YEAR,
        IMM_MONTH,
        IMM_QUARTER,
        MONTH_EOM
    };

    CDateInterval();
    CDateInterval(int numPeriods, EPeriodType periodType);
    CDateInterval(const std::string&);
    std::string ToString() const;
    operator std::string() const;
    operator bool() const;
    bool IsDaily() const;
    bool IsIMM() const;
    bool IsMonthly() const;

    int NumDays() const;
    int NumMonths() const;
    double Years() const;
    double Frequency() const;

    int NumPeriods() const;
    EPeriodType PeriodType() const;

    CDate AddToDate(CDate startDate, int count=1) const;

private:
    void VerifyAndComplete();
    int  m_numPeriods;
    EPeriodType m_periodType;
};

std::vector<CDate> GenerateDates(
    CDate startDate,
    CDate endDate,
    const CDateInterval& interval,
    bool stubAtEnd);

CDate operator+ (CDate startDate, const CDateInterval& ivl);
CDate operator- (CDate startDate, const CDateInterval& ivl);

#endif


