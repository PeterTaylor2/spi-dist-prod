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
#ifndef CPPLIB_HOLIDAYS_HPP
#define CPPLIB_HOLIDAYS_HPP

#include "Date.hpp"
#include "Declare.hpp"
#include <map>
#include <vector>

/*
** These constants define flags for determining whether a day is a weekend
** or not. This is to cope with countries for which weekends fall on Fridays
** and Saturdays etc.
**
** Note the holiday file "NONE" corresponds to an empty date list and
** CPPLIB_WEEKEND_STANDARD, whereas the holiday file "NO_WEEKENDS" corresponds
** to an empty date list and CPPLIB_WEEKEND_NO_WEEKENDS.
**
** By default when reading holidays from file, the weekend status will be
** CPPLIB_WEEKEND_STANDARD unless specifically over-ridden.
*/
#define CPPLIB_WEEKEND_MONDAY      0x0001
#define CPPLIB_WEEKEND_TUESDAY     0x0002
#define CPPLIB_WEEKEND_WEDNESDAY   0x0004
#define CPPLIB_WEEKEND_THURSDAY    0x0008
#define CPPLIB_WEEKEND_FRIDAY      0x0010
#define CPPLIB_WEEKEND_SATURDAY    0x0020
#define CPPLIB_WEEKEND_SUNDAY      0x0040
#define CPPLIB_WEEKEND_NO_WEEKENDS 0x0000
#define CPPLIB_WEEKEND_STANDARD    (CPPLIB_WEEKEND_SATURDAY | CPPLIB_WEEKEND_SUNDAY)

/**
***************************************************************************
** Determines whether a day is at a weekend as given by the weekends flag.
***************************************************************************
*/
bool IsWeekend(CDate date, long weekends=CPPLIB_WEEKEND_STANDARD);

/**
***************************************************************************
** Determines whether a day is a weekday as given by the weekends flag.
***************************************************************************
*/
bool IsWeekDay(CDate date, long weekends=CPPLIB_WEEKEND_STANDARD);

/**
****************************************************************************
** Bad day convention
****************************************************************************
*/
enum EBadDayConv : int
{
    /// Use following good day
    CPPLIB_BAD_DAY_FOLLOW, 

    /// Use previous good day
    CPPLIB_BAD_DAY_PREVIOUS,

    /// Don't check for bad days
    CPPLIB_BAD_DAY_NONE,

    /// Try following, then previous if a month boundary is crossed
    CPPLIB_BAD_DAY_MODIFIED 
};

EBadDayConv BadDayConvFromString(const char*);
EBadDayConv BadDayConvFromString(const std::string&);
const std::string& BadDayConvToString(EBadDayConv badDayConv);

/**
***************************************************************************
** Contains holiday dates and a series of flags that indicate that
** particular days of the week are always holidays.
***************************************************************************
*/
CPPLIB_DECLARE_CLASS(CHolidayList);
typedef std::map<std::string, CHolidayListSP> CHolidayCache;

class CHolidayList
{
public:
    CHolidayList(const std::vector<CDate>& dateList, long weekends);

    const std::vector<CDate>& DateList() const { return m_dateList; }
    long Weekends() const { return m_weekends; }
    const std::vector<CDate>& WeekdayList() const { return m_weekdayList; }

    /**
    ***************************************************************************
    ** Reads a holiday file into memory as a holiday list.
    **
    ** This is the J.P.Morgan style holiday file also supported by ISDA/CDS.
    **
    ** Structure of holiday file, ascii text file of lines like:
    **     #            - commment (blank lines are also skipped)
    **     19631026     - means 1963-10-26 is a holiday
    **     # SATURDAY_NOT_ALWAYS_HOLIDAY  - sets "saturday isn't always a holiday"
    **     # SUNDAY_NOT_ALWAYS_HOLIDAY    - sets "sunday isn't always a holiday"
    **     # MONDAY_ALWAYS_HOLIDAY        - sets "monday as always a holiday"
    **     # TUESDAY_ALWAYS_HOLIDAY       - sets "tuesday as always a holiday"
    **     # WEDNESDAY_ALWAYS_HOLIDAY     - sets "wednesday as always a holiday"
    **     # THURSDAY_ALWAYS_HOLIDAY      - sets "thursday as always a holiday"
    **     # FRIDAY_ALWAYS_HOLIDAY        - sets "friday as always a holiday"
    **
    ** Dates must be in increasing order.
    ***************************************************************************
    */
    static CHolidayListSP Read(const std::string& fileName);

    /**
    ***************************************************************************
    ** Calculates a business date being at offset business days
    ** from the original date
    ***************************************************************************
    */
    CDate AddBusinessDays(CDate fromDate, int offset);

    /**
    ***************************************************************************
    ** Using business day conventions (Following, Preceding, and Modified
    ** Following) calculates the next bisiness day if the input date is not a
    ** business day. Input and Output dates are represented as TDate types.
    ***************************************************************************
    */
    CDate BusinessDay(CDate date, EBadDayConv method);

    /**
    ***************************************************************************
    ** Indicates whether a date is a business day.
    ***************************************************************************
    */
    bool IsBusinessDay(CDate date);

    /**
    ***************************************************************************
    ** Indicates whether a date is a holiday.
    **
    ** Note that this function can return true if the date provided is 
    ** a holiday and also falls on the weekend unless you set excludeWeekends.
    ***************************************************************************
    */
    bool IsHoliday(CDate date, bool excludeWeekends = false);

    /**
    ***************************************************************************
    ** Calculates the number of business days between two dates (FROM & TO).
    **
    ** Algorithm:
    ** 1. if FROM = TO, the result is 0
    ** 2. if FROM < TO, the result is the number of business days in the CLOSED
    **    interval of [FROM+1,TO]
    ** 3. if FROM > TO, the result is negated number of business days in the
    **    CLOSED interval of [TO,FROM-1]
    ***************************************************************************
    */
    int BusinessDaysDiff(CDate fromDate, CDate toDate);

    /**
    ***************************************************************************
    ** Computes the last business day of the month.
    ***************************************************************************
    */
    CDate BusinessEndOfMonth(CDate date);

private:
    std::vector<CDate> m_dateList;
    long m_weekends;

    std::vector<CDate> m_weekdayList; // with weekends filtered out
    void VerifyAndComplete();
};

/**
***************************************************************************
** If the holiday is in the cache then it is returned.
** Otherwise it treats the name as a filename and attempts to load it.
**
** Note that NONE and NO_WEEKENDS are pre-loaded into the cache.
**
** Note that the cache names are case-insensitive, but when reading a file
** we use the name given to try and read the file.
***************************************************************************
*/
CHolidayListSP HolidayListFromCache(const std::string& name);

/**
***************************************************************************
** Adds a holiday list to the holiday cache. If the entry already exists
** in the cache, then the old version will be removed.
***************************************************************************
*/
void HolidayListAddToCache(
    const std::string& name,
    const CHolidayListSP& hl);

/**
***************************************************************************
** Loads a holiday cache entry from a disk file, removing old one if it
** exists.
***************************************************************************
*/
void HolidayLoadFromDisk(
    const std::string& name,
    const std::string& fileName);

/**
***************************************************************************
** Clear the holiday cache
***************************************************************************
*/
void HolidayCacheClear();

/*
*************************************************************************
** Functions using a string to represent the holidays.
**
** We reserve the information to change string to a structure which can
** be coerced automatically from string at a later date by using typedef.
**************************************************************************
*/
typedef std::string Holidays;


/**
***************************************************************************
** Calculates a business date being at offset business days
** from the original date
***************************************************************************
*/
CDate AddBusinessDays(
    CDate fromDate,
    int   offset,
    const Holidays& holidays);

/**
***************************************************************************
** Using business day conventions (Following, Preceding, and Modified
** Following) calculates the next bisiness day if the input date is not a
** business day. Input and Output dates are represented as TDate types.
***************************************************************************
*/
CDate BusinessDay
(CDate           date,  
 EBadDayConv     method,  
 const Holidays& holidays);

/**
***************************************************************************
** Indicates whether a date is a business day.
***************************************************************************
*/
bool IsBusinessDay(
    CDate date,
    const Holidays& holidays);

/**
***************************************************************************
** Indicates whether a date is a holiday.
***************************************************************************
*/
bool IsHoliday(
    CDate date,
    const Holidays& holidays,
    bool excludeWeekends=false);

/**
***************************************************************************
** Calculates the number of business days between two dates (FROM & TO).
**
** Algorithm:
** 1. if FROM = TO, the result is 0
** 2. if FROM < TO, the result is the number of business days in the CLOSED
**    interval of [FROM+1,TO]
** 3. if FROM > TO, the result is negated number of business days in the
**    CLOSED interval of [TO,FROM-1]
***************************************************************************
*/
int BusinessDaysDiff
    (CDate         fromDate,     /* (I) Earlier date       */
     CDate         toDate,       /* (I) Later date         */
     const Holidays& holidays);


/*f
***************************************************************************
** Computes the last business day of the month.
***************************************************************************
*/
CDate BusinessEndOfMonth
    (CDate date,
     const Holidays& holidays);


#endif    /* CPPLIB_HOLIDAYS_HPP */

