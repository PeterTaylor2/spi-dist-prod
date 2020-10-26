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

// some of the optimizations were provided by the original ISDA/CDS code
// hence the following copyright notice

/*
 * ISDA CDS Standard Model
 *
 * Copyright (C) 2009 International Swaps and Derivatives Association, Inc.
 * Developed and supported in collaboration with Markit
 * 
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the ISDA CDS Standard Model Public License.
 */


#include "Holidays.hpp"
#include "RuntimeError.hpp"
#include "EnumLookup.hpp"

#include <fstream>
#include <spi_util/StringUtil.hpp>
#include <spi_util/StreamUtil.hpp>

/*
***************************************************************************
** Implementation of CHolidayCache
***************************************************************************
*/
namespace 
{
    CHolidayCache g_holidayCache;
}

CHolidayListSP HolidayListFromCache(const std::string& name)
{
    PRE_CONDITION(name.length() > 0);
    
    if (g_holidayCache.empty())
    {
        std::vector<CDate> noDates;
        g_holidayCache["NONE"] = Share(new CHolidayList(noDates, CPPLIB_WEEKEND_STANDARD));
        g_holidayCache["NO_WEEKENDS"] = Share(new CHolidayList(noDates, CPPLIB_WEEKEND_NO_WEEKENDS));
    }

    std::string ucName = spi_util::StringUpper(name);

    CHolidayCache::iterator iter = g_holidayCache.find(ucName);
    if (iter != g_holidayCache.end())
        return iter->second;

    CHolidayListSP hl = CHolidayList::Read(name);
    g_holidayCache[ucName] = hl;

    return hl;
}

void HolidayListAddToCache(
    const std::string& name,
    const CHolidayListSP& hl)
{
    PRE_CONDITION(name.length() > 0);
    PRE_CONDITION(hl);

    std::string ucName = spi_util::StringUpper(name);

    PRE_CONDITION(ucName != "NONE");
    PRE_CONDITION(ucName != "NO_WEEKENDS");

    g_holidayCache[ucName] = hl;
}

void HolidayLoadFromDisk(
    const std::string& name,
    const std::string& fileName)
{
    HolidayListAddToCache(name, CHolidayList::Read(fileName));
}

void HolidayCacheClear()
{
    g_holidayCache.clear();
}


/*
****************************************************************************
** Implementation of Weekends
****************************************************************************
*/
bool IsWeekend(CDate date, long weekends)
{
    int isWeekEnd = ((1 << (((int)date) % 7)) & (weekends));

    return isWeekEnd != 0;
}

bool IsWeekDay(CDate date, long weekends)
{
    return !IsWeekend(date, weekends);
}

/*
*****************************************************************************
** Implementation of EBadDayConv
*****************************************************************************
*/
namespace
{
    CEnumLookup* BadDayConvLookup()
    {
        static CEnumLookup enumLookup("BadDayConv");
        static bool init = false;

        if (!init)
        {
            enumLookup.AddItem(CPPLIB_BAD_DAY_FOLLOW, "Follow");
            enumLookup.AddItem(CPPLIB_BAD_DAY_FOLLOW, "F");
            enumLookup.AddItem(CPPLIB_BAD_DAY_PREVIOUS, "Previous");
            enumLookup.AddItem(CPPLIB_BAD_DAY_PREVIOUS, "P");
            enumLookup.AddItem(CPPLIB_BAD_DAY_NONE, "None");
            enumLookup.AddItem(CPPLIB_BAD_DAY_NONE, "N");
            enumLookup.AddItem(CPPLIB_BAD_DAY_MODIFIED, "Modified");
            enumLookup.AddItem(CPPLIB_BAD_DAY_MODIFIED, "M");
            init = true;
        }

        return &enumLookup;
    }
}

EBadDayConv BadDayConvFromString(const char* str)
{
    return (EBadDayConv)(BadDayConvLookup()->FromString(str));
}

EBadDayConv BadDayConvFromString(const std::string& str)
{
    return (EBadDayConv)(BadDayConvLookup()->FromString(str.c_str()));
}

const std::string& BadDayConvToString(EBadDayConv badDayConv)
{
    return BadDayConvLookup()->ToString((int)badDayConv);
}


/*
***************************************************************************
** Implementation of CHolidayList
***************************************************************************
*/
namespace 
{
    int offsetTable[7][5] =
    {/*n:  0  1  2  3  4 */

        {  0, 1, 2, 3, 4 },  /* Monday */
        {  0, 1, 2, 3, 6 },  /* Tuesday */
        {  0, 1, 2, 5, 6 },  /* Wednesday */
        {  0, 1, 4, 5, 6 },  /* Thursday */
        {  0, 3, 4, 5, 6 },  /* Friday */
        { -1, 2, 3, 4, 5 },  /* Saturday */
        { -2, 1, 2, 3, 4 }   /* Sunday */
    };

    int SIGN(int x)
    {
        if (x < 0)
            return -1;
        return 1;
    }

    int ABS(int x)
    {
        if (x < 0)
            return -x;
        return x;
    }

    /*
    ***************************************************************************
    ** This function adjusts a forwarded date to take account of any holidays 
    ** which might have occured during the forward calculation.
    **
    ** The algorithm assumes the following conditions:
    **
    ** (1) Saturdays and Sundays are both holidays.
    ** (2) The direction is forwards in time.
    ** (3) We have already moved forward the requisite number of weekdays.
    **
    ** The function is optimized for speed.
    ***************************************************************************
    */
    CDate adjustStandardWeekends (
        CDate fromDate,
        const std::vector<CDate>& dateList,
        CDate originalResult)
    {
        CDate result = originalResult;

        // there are no holidays - no further adjustments
        if (dateList.size() == 0)
            return result;

        // fromDate is on or after the end of the list of holidays - no further adjustment
        // note that we don't count holidays that are on the original date
        if (fromDate >= dateList.back())
            return result;

        // we use upper_bound since we don't count holidays that are on the original date
        std::vector<CDate>::const_iterator iter = std::upper_bound(
            dateList.begin(), dateList.end(), fromDate);

        POST_CONDITION(iter != dateList.end()); // since fromDate < dateList.back()
        POST_CONDITION(*iter > fromDate); // advertised property of upper_bound

        while (iter != dateList.end() && *iter <= result)
        {
            // if we find a holiday between fromDate and result then we need to go
            // forward one weekday
            //
            // the offsetTable is set-up so that we move forward the correct number
            // of days on the assumption of standard weekends
            int offset = offsetTable[ ((int)result) % 7 ] [1];
            result += offset;
            ++iter;
        }

        return result;
    }


    /*
    ***************************************************************************
    ** This function does the following:
    **
    ** If going forward in time, it finds the index of the 1st holiday date
    ** which is >= the input date.
    **
    ** If going backwards in time, it finds the index of the first of holiday
    ** which is <= the input date.
    ***************************************************************************
    */
    void findFirstHolidayIndex(
        CDate date,
        const std::vector<CDate>& dateList,
        int   direction,
        int&  index,
        bool& outOfBounds)
    {
        PRE_CONDITION(dateList.size() > 0);

        int       numHols = (int)dateList.size();

        /* Check if the date is outside array bounds. */
        outOfBounds = ( (direction ==  1 && dateList[numHols-1] < date) ||
                         (direction == -1 && dateList[0] > date) );

        if (outOfBounds)
        {
            index = -1;
            return;
        }

        std::vector<CDate>::const_iterator iter;

        if (direction == 1)
        {
            // forward
            // looking for index of first holiday >= date, i.e. not less than, i.e. lower_bound

            iter = std::lower_bound(dateList.begin(), dateList.end(), date);
            
            POST_CONDITION(iter != dateList.end()); // !outOfBonds
            POST_CONDITION(*iter >= date);
        }
        else
        {
            // backward
            // looking for index of first holiday <= date
            // this is the one before the first holiday > date, i.e. upper_bound

            iter = std::upper_bound(dateList.begin(), dateList.end(), date);

            POST_CONDITION(iter != dateList.begin()); // !outOfBounds
            --iter;
            POST_CONDITION(*iter <= date);
        }

        index = iter - dateList.begin();
    }

    
    /*
    ***************************************************************************
    ** Compute the number of week-day holidays starting at the input holiday 
    ** index and continuing until the holiday list is at or beyond toDate.
    **
    ** Week-day holidays are holidays which occur on a non-weekend day.
    ***************************************************************************
    */
    int countHolidays (
        CDate              toDate,     /* (I) End date.   */
        int                startIndex,/* (I) Starting holiday index. */
        int                direction,  /* (I) 1=forwards, 2=backwards*/
        long               weekends,   /* (I) Weekend flags */
        const std::vector<CDate>& dateList, /* (I) Holiday datelist. */
        int&               endIndex   /* (O) Idx where hol=toDate, +1 */
    )
    {
        int i;
        int count = 0;

        int numDates = (int)dateList.size();

        if (direction == 1)     /* forward */
        {
            // is this more efficient than lower_bound()?
            for (i = startIndex; i < numDates && dateList[i] <= toDate; ++i)
            {
                ++count;
            }

            endIndex = i;

            if (i < numDates && dateList[i] == toDate)
                ++i;
        }
        else                   /* backward */
        {
            for (i = startIndex; i >= 0 && dateList[i] >= toDate; --i)
            {
                ++count;
            }

            endIndex = i;
            if (i >= 0 && dateList[i] == toDate)
                --i;
        }

        return count;
    }

    /*
    ***************************************************************************
    ** This function deals with the case that we are moving forward a number
    ** of business days with non-standard weekends.
    ***************************************************************************
    */
    CDate forwardNonStandardWeekends (
        CDate fromDate,       /* (I) start date */
        int   numBusDays,     /* (I) abs. num. bus. days */
        int   direction,      /* (I) +1=forwards, -1=backwards */
        long  weekends,       /* (I) Weekends flags */
        int   busDaysPerWeek, /* (I) num. bus. days per week */
        const std::vector<CDate>& dateList     /* (I) holiday dateList */
    )
    {
        int    numHolidays = 0;
        CDate  curDate = fromDate;
        int    holIdx = -1;
        bool   doneSearchingList = false;

        int numHols = (int)dateList.size();

        /*
        ** First, adjust for weekends only, pretending there are no holidays.
        */
        int numWeeks = std::max(((numBusDays / busDaysPerWeek) - 1), 0);
        curDate += (7 * numWeeks * direction);

        if (dateList.size() > 0)
        {
            /*
            ** Search the holiday list for the first holiday
            ** strictly after (if going forward in time) or strictly
            ** before (if going backward in time) the start date. Note
            ** that the holiday list is assumed to be sorted in increasing
            ** order.
            */
            findFirstHolidayIndex(fromDate + direction,
                dateList,
                direction,
                holIdx,
                doneSearchingList);
        }
        else
        {
            doneSearchingList = true;
        }

        /*
        ** Count the number of weekday holidays starting from the
        ** current holiday index and going up to the curDate.
        ** Count weekday holidays only because holidays occurring
        ** on week-end days have been skipped by the previous calculation.
        */
        if (!doneSearchingList)
        {
            numHolidays = countHolidays (curDate,
                                         holIdx,
                                         direction,
                                         weekends,
                                         dateList,
                                         holIdx);

            doneSearchingList = (holIdx  <  0  ||  holIdx  >=  numHols);
        }
        else
        {
            numHolidays = 0;
        }
        numBusDays -= ((busDaysPerWeek * numWeeks) - numHolidays);

        /*
        ** Now search one day at a time starting one day beyond
        ** the current date.
        */
        while (numBusDays > 0)
        {
            curDate += direction;

            /*
            ** Check if the day is a holiday first. If it is, don't decrement
            ** numBusDays.
            */
            if (!doneSearchingList && curDate == dateList[holIdx])
            {
                holIdx += direction;
                doneSearchingList = (holIdx  <  0  ||  holIdx  >=  numHols);
            }
            else  /* Not a holiday. */
            {
                /*
                ** If the day is a weekday, then decrement numBusDays,
                ** otherwise continue looping.
                */
                if (IsWeekDay(curDate, weekends))
                    --numBusDays;
            }
        }

        return curDate;
    }



    /*
    ***************************************************************************
    ** Compute the date of next business day, given a start date and
    ** a holiday list. 
    ***************************************************************************
    */
    CDate nextBusinessDate
        (CDate          startDate,    /* (I) starting date. */
         int            direction,    /* (I) +1=forwards, -1=backwards */
         const std::vector<CDate> dateList,
         long weekends)
    {
        bool doneSearchingList;
        int index;
        int numHols = (int)dateList.size();

        if (numHols == 0)
        {
            doneSearchingList = true;
        }
        else
        {
            findFirstHolidayIndex(startDate, dateList, direction,
                index, doneSearchingList);
        }

        /*
        ** index should now point to the first holiday on or before
        ** (after) the start date. Now loop though each day until
        ** you reach the first business day.
        */

        bool aHoliday = true;
        CDate date = startDate;
        do
        {   /* Check if it's a holiday first. */
            if (!doneSearchingList && date == dateList[ index ])
            {
                index  += direction;
                date   += direction;
                doneSearchingList = (index < 0 || index >= numHols);
            }
            else
            {
                if (IsWeekend(date, weekends))
                {
                    date += direction;
                }
                else
                {
                    aHoliday = false;
                }
            }
        }
        while (aHoliday);

        return date;
    }
}

CHolidayList::CHolidayList(
    const std::vector<CDate>& dateList,
    long weekends)
    :
    m_dateList(dateList),
    m_weekends(weekends)
{
    VerifyAndComplete();
}

void CHolidayList::VerifyAndComplete()
{
    PRE_CONDITION(m_weekends >= 0);
    PRE_CONDITION(m_weekends <= 0x07F);

    size_t nbDates = m_dateList.size();
    m_weekdayList.clear();

    for (size_t i = 1; i < nbDates; ++i)
    {
        const CDate& thisDay = m_dateList[i];
        const CDate& lastDay = m_dateList[i-1];
        if (thisDay <= lastDay)
        {
            THROW_RUNTIME_ERROR("Dates are not in strictly increasing order.\n"
                << "    [" << i << "] = " << thisDay.ToString() << "; [" << (i-1) << "] = "
                << lastDay.ToString());
        }

        /* remove weekends */
        if (IsWeekDay(thisDay, m_weekends))
            m_weekdayList.push_back(thisDay);
    }
}

CHolidayListSP CHolidayList::Read(
    const std::string& fileName)
{
    PRE_CONDITION(fileName.length() > 0);

    try
    {

        std::ifstream fp(fileName.c_str());
        if (!fp)
            THROW_RUNTIME_ERROR("Could not open file " << fileName);

        std::vector<CDate> dateList;
        long weekends = CPPLIB_WEEKEND_STANDARD;

        std::vector<std::string> lines = spi_util::StreamReadLines(fp);
        size_t nbLines = lines.size();

        for (size_t i = 0; i < nbLines; ++i)
        {
            const std::string& line = spi_util::StringStrip(lines[i]);

            if (line.length() == 0)
                continue; // ignore blank lines

            if (line[0] == '#')
            {
                if (line == "# MONDAY_ALWAYS_HOLIDAY")
                    weekends |= CPPLIB_WEEKEND_MONDAY;
                else if (line == "# TUESDAY_ALWAYS_HOLIDAY")
                    weekends |= CPPLIB_WEEKEND_TUESDAY;
                else if (line == "# WEDNESDAY_ALWAYS_HOLIDAY")
                    weekends |= CPPLIB_WEEKEND_WEDNESDAY;
                else if (line == "# THURSDAY_ALWAYS_HOLIDAY")
                    weekends |= CPPLIB_WEEKEND_THURSDAY;
                else if (line == "# FRIDAY_ALWAYS_HOLIDAY")
                    weekends |= CPPLIB_WEEKEND_FRIDAY;
                else if (line == "# SATURDAY_NOT_ALWAYS_HOLIDAY")
                    weekends &= ~CPPLIB_WEEKEND_SATURDAY;
                else if (line == "# SUNDAY_NOT_ALWAYS_HOLIDAY")
                    weekends &= ~CPPLIB_WEEKEND_SUNDAY;
            }
            else
            {
                long yyyymmdd = atol(line.c_str()); // we ignore any text at end of number

                if (yyyymmdd > 16010101)
                {
                    int year  = yyyymmdd / 10000;
                    int month = (yyyymmdd - year * 10000) / 100;
                    int day   = yyyymmdd % 100;

                    CDate date;

                    try
                    {
                        date = CDate(year, month, day);
                    }
                    catch (std::exception& e)
                    {
                        RETHROW(e, "Invalid date: " << line);
                    }

                    dateList.push_back(date); // check order when constructing HolidayList
                }
            }
        }
        // check that we have some content...
        if (dateList.size() == 0 && weekends == CPPLIB_WEEKEND_STANDARD)
        {
            THROW_RUNTIME_ERROR("No holiday information found in " << fileName << ".\n"
                << " - either week-end information or dates must be provided.");
        }

        return Share(new CHolidayList(dateList, weekends));
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

CDate CHolidayList::AddBusinessDays(CDate fromDate, int offset)
{
    int intervalSign = SIGN(offset);
    int numBusDays   = ABS(offset);
    int busDaysPerWeek = -1;

    CDate result;

    /*
    ** Get the number of business days per week.
    */
    switch (m_weekends)
    {
    case CPPLIB_WEEKEND_STANDARD:
        busDaysPerWeek = 5;
        break;
    case CPPLIB_WEEKEND_NO_WEEKENDS:
        busDaysPerWeek = 7;
        break;
    default:
        busDaysPerWeek = 7;
        if (m_weekends & CPPLIB_WEEKEND_SUNDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_MONDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_TUESDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_WEDNESDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_THURSDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_FRIDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_SATURDAY) --busDaysPerWeek;
        break;
    }

    if (m_dateList.size() == 0 && m_weekends == CPPLIB_WEEKEND_NO_WEEKENDS)
    {
        // no adjustments - essentially NO_WEEKENDS
        result = fromDate + offset;
    }
    else if (intervalSign == 1 && m_weekends == CPPLIB_WEEKEND_STANDARD)
    {
        // special case (and in fact normal case)
        if (numBusDays == 0)
        {
            result = fromDate;
        }
        else
        {
           /*
            * Calculate result if no holidays are involved.
            *
            * Move forward over a week for every 5 business days.
            * Use a table for moving 0..4 bus days from each day of the week.
            */
            result = fromDate +  7 * (numBusDays / 5) +
                             offsetTable[ ((int)fromDate) % 7 ][ numBusDays % 5 ];

            if (m_dateList.size() > 0)
            {
                result = adjustStandardWeekends(fromDate, m_dateList, result);
            }
        }
    }
    else
    {
        result = forwardNonStandardWeekends (fromDate,
                                             numBusDays,
                                             intervalSign,
                                             m_weekends,
                                             busDaysPerWeek,
                                             m_dateList);
    }
    return result;
}

CDate CHolidayList::BusinessDay(CDate date, EBadDayConv badDayConv)
{
    CDate nextDate;
    switch(badDayConv)
    {
    case CPPLIB_BAD_DAY_NONE:
        return date;
    case CPPLIB_BAD_DAY_FOLLOW:
        nextDate = nextBusinessDate(date, 1, m_weekdayList, m_weekends);
        return nextDate;
    case CPPLIB_BAD_DAY_PREVIOUS:
        nextDate = nextBusinessDate(date, -1, m_weekdayList, m_weekends);
        return nextDate;
    case CPPLIB_BAD_DAY_MODIFIED:
        nextDate = nextBusinessDate(date, 1, m_weekdayList, m_weekends);
        if (nextDate.Month() != date.Month())
        {
            nextDate = nextBusinessDate(date, -1, m_weekdayList, m_weekends);
        }
        return nextDate;
    }

    THROW_RUNTIME_ERROR("Unrecognized bad day convention = " << (int)badDayConv);
}

bool CHolidayList::IsBusinessDay(CDate date)
{
    if (IsWeekend(date, m_weekends))
        return false;

    bool isHoliday = IsHoliday(date);
    return !isHoliday;
}

bool CHolidayList::IsHoliday(CDate date, bool excludeWeekends)
{
    const std::vector<CDate>& dateList = excludeWeekends ? m_weekdayList : m_dateList;

    std::vector<CDate>::const_iterator iter = std::lower_bound(
        dateList.begin(), dateList.end(), date);

    if (iter == dateList.end())
        return false;

    POST_CONDITION(*iter >= date);

    return *iter == date;
}

int CHolidayList::BusinessDaysDiff(CDate fromDate, CDate toDate)
{
    /*
     * The following tables are used to calculate the number of business days
     * between weekdays.
     * If (toDate > fromDate) use fwdDiffTable.
     * eg NumDays = fwdDiffTable[ fromDate % 7 ][ toDate % 7 ]
     *
     * If (fromDate > toDate) use -bwdDiffTable.
     * eg NumDays = bwdDiffTable[ toDate % 7 ][ fromDate % 7 ]
     *
     * The values of the table were chosen so that the current ALIB behaviour
     * was left unchanged. Note that because fwdDiffTable is not identical
     * to (-1 x bwdDiffTable), swapping fromDate and toDate in the call to this
     * function does not simply reverse the sign of the result.
     */
    static int fwdDiffTable[7][7] =
        {/*   Mo Tu We Th Fr Sa Su*/

            {  0, 1, 2, 3, 4, 4, 4 },  /* Monday */
            {  4, 0, 1, 2, 3, 3, 3 },  /* Tuesday */
            {  3, 4, 0, 1, 2, 2, 2 },  /* Wednesday */
            {  2, 3, 4, 0, 1, 1, 1 },  /* Thursday */
            {  1, 2, 3, 4, 0, 0, 0 },  /* Friday */
            {  1, 2, 3, 4, 5, 0, 0 },  /* Saturday */
            {  1, 2, 3, 4, 5, 5, 0 }   /* Sunday */
        };

    static int bwdDiffTable[7][7] =
        {/*   Mo Tu We Th Fr Sa Su*/

            {  0,-1,-2,-3,-4,-5,-5 },  /* Monday */
            { -4, 0,-1,-2,-3,-4,-4 },  /* Tuesday */
            { -3,-4, 0,-1,-2,-3,-3 },  /* Wednesday */
            { -2,-3,-4, 0,-1,-2,-2 },  /* Thursday */
            { -1,-2,-3,-4, 0,-1,-1 },  /* Friday */
            { -0,-1,-2,-3,-4, 0, 0 },  /* Saturday */
            { -0,-1,-2,-3,-4,-5, 0 }   /* Sunday */
        };

    if (fromDate == toDate)
        return 0;

    int direction = toDate < fromDate ? -1 : 1;

    /*
    ** First, compute the date difference adjusting for weekends only.
    */
    CDate date = fromDate;
    int result = 0;
    int nrExtraDays = 0;
    int busDaysPerWeek;
    int numWeeks;

    /*
    ** Get the number of business days per week. In-line for speed.
    */
    switch (m_weekends)
    {
    case CPPLIB_WEEKEND_STANDARD:
        busDaysPerWeek = 5;
        numWeeks = (toDate - fromDate) / 7;
        date += 7 * numWeeks;
        if (date > toDate)  /* backwards */
        {
            nrExtraDays = bwdDiffTable [ ((int)toDate) %7 ][ ((int)fromDate) % 7 ];
        }
        else
        {
            nrExtraDays = fwdDiffTable [ ((int)fromDate) %7 ][ ((int)toDate) % 7 ];
        }
        result = numWeeks * busDaysPerWeek + nrExtraDays;
        break;
    case CPPLIB_WEEKEND_NO_WEEKENDS:
        result = toDate - fromDate;
        break;
    default:
        busDaysPerWeek = 7;
        if (m_weekends & CPPLIB_WEEKEND_SUNDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_MONDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_TUESDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_WEDNESDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_THURSDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_FRIDAY) --busDaysPerWeek;
        if (m_weekends & CPPLIB_WEEKEND_SATURDAY) --busDaysPerWeek;

        numWeeks   = ABS((toDate - fromDate) / 7);
        date       += (7 * numWeeks * direction);
        nrExtraDays = 0;

        while ( ABS(date - toDate) > 0 )
        {
            date += direction;
            if (IsWeekDay( date, m_weekends))
                ++nrExtraDays;
        }

        result = ((numWeeks * busDaysPerWeek) + nrExtraDays) * direction;
        break;
    }


    /*
    ** Now count the number of weekday holidays
    ** and adjust the date difference by the result.
    */
    const std::vector<CDate>& dateList = m_weekdayList;

    if (dateList.size() > 0)
    {
        int index;
        bool outOfBounds;

        findFirstHolidayIndex(fromDate + direction, dateList, direction,
            index, outOfBounds);

        if (!outOfBounds)
        {
            int numHolidays = countHolidays(toDate, index, direction,
                m_weekends, dateList, index);

            result -= (direction * numHolidays);
        }
    }

    return result;
}


CDate CHolidayList::BusinessEndOfMonth(CDate date)
{
    return BusinessDay(date.EndOfMonth(), CPPLIB_BAD_DAY_PREVIOUS);
}

/*
******************************************************************************
** Implementation of stand-alone business day functions which use holidays
** to get a HolidayList and then call the corresponding method.
******************************************************************************
*/
CDate AddBusinessDays(
    CDate fromDate,
    int   offset,
    const Holidays& holidays)
{
    try
    {
        CHolidayListSP hl = HolidayListFromCache(holidays);
        return hl->AddBusinessDays(fromDate, offset);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

CDate BusinessDay
(CDate           date, 
 EBadDayConv     method,  
 const Holidays& holidays)
{
    try
    {
        CHolidayListSP hl = HolidayListFromCache(holidays);
        return hl->BusinessDay(date, method);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

bool IsBusinessDay
(CDate           date, 
 const Holidays& holidays)
{
    try
    {
        CHolidayListSP hl = HolidayListFromCache(holidays);
        return hl->IsBusinessDay(date);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

bool IsHoliday
(CDate           date, 
 const Holidays& holidays,
 bool            excludeWeekends)
{
    try
    {
        CHolidayListSP hl = HolidayListFromCache(holidays);
        return hl->IsHoliday(date, excludeWeekends);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

int BusinessDaysDiff
    (CDate         fromDate,     /* (I) Earlier date       */
     CDate         toDate,       /* (I) Later date         */
     const Holidays& holidays)
{
    try
    {
        CHolidayListSP hl = HolidayListFromCache(holidays);
        return hl->BusinessDaysDiff(fromDate, toDate);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

CDate BusinessEndOfMonth
(CDate           date, 
 const Holidays& holidays)
{
    try
    {
        CHolidayListSP hl = HolidayListFromCache(holidays);
        return hl->BusinessEndOfMonth(date);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

