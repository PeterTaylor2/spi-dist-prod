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
#ifndef CPPLIB_DATE_HPP
#define CPPLIB_DATE_HPP

/**
***************************************************************************
** Date.hpp
***************************************************************************
** Date class which uses internally the number of days since some arbitrary
** base date. 
***************************************************************************
*/

#include <string>

enum EWeekday : char
{
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

const char* WeekdayName(EWeekday weekday);

/**
 * Date class contains no virtual functions - hence the size of the class
 * is exactly the same as the size of its data fields - hence we go for
 * minimality, so that a vector of Date is as cheap as possible (for
 * example).
 */
class CDate
{
public:
    // constructors
    CDate();
    CDate(int dt);
    CDate(int year, int month, int day);
    static CDate FromString(const std::string& str);
    static CDate Today();
    // access methods
    void YMD(int* year, int* month, int* day) const;
    int Year() const;
    int Month() const;
    int Day() const;
    EWeekday Weekday() const;
    CDate EndOfMonth() const;
    CDate StartOfMonth() const;
    operator int() const;
    std::string ToString() const;
    operator std::string() const;

    // in addition we will need functions which act on dates
    // e.g. add a number of days, months (pay attention to end of month
    //      adjustment rules), years (as above), subtract two dates to
    //      give a number of days
    //
    // most of these should be outside the class - e.g. operator + and
    // operator - for adding days or getting the number of days between
    // two dates

    CDate& operator += (int days);
    CDate& operator -= (int days);

private:
    int m_dt;
};

// simple CDate arithmetic
// it is possible that we don't really need all the comparison methods here
// however it should help to avoid ambiguity

inline bool operator == (CDate d1, CDate d2)
{
    return (int)d1 == (int)d2;
}

inline bool operator != (CDate d1, CDate d2)
{
    return (int)d1 != (int)d2;
}

inline bool operator < (CDate d1, CDate d2)
{
    return (int)d1 < (int)d2;
}

inline bool operator > (CDate d1, CDate d2)
{
    return (int)d1 > (int)d2;
}

inline bool operator <= (CDate d1, CDate d2)
{
    return (int)d1 <= (int)d2;
}

inline bool operator >= (CDate d1, CDate d2)
{
    return (int)d1 >= (int)d2;
}

// difference between two dates is returned as an integer number of days

inline int operator - (CDate d1, CDate d2)
{
    return (int)d1 - (int)d2;
}

// we can add a number of days and get another date

inline CDate operator + (CDate dt, int days)
{
    return CDate((int)dt + days);
}

// we can subtract a number of days and get another date

inline CDate operator - (CDate dt, int days)
{
    return CDate((int)dt - days);
}

bool IsLeapYear(int year);
int LastDayInMonth(int year, int month);
CDate NthWeekdayInMonth(int year, int month, int N, EWeekday weekday);

#endif


