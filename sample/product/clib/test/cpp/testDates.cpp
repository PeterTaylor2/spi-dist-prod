/*
***************************************************************************
** Tests of all the functions and classes defined in dates.cfg
***************************************************************************
*/

#include "clib_dateType.hpp"
#include "clib_dates.hpp"
#include "clib_clib_service.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdio.h>

#include <spi/TestRunner.hpp>
#include <spi/RuntimeError.hpp>

static void testDayOfWeek(void)
{
    spi::Date date = clib::Date(2014,10,5); // which is Sunday
    std::cout << "DayOfWeek(" << date.ToString() << ") = " << clib::DayOfWeek(date) << std::endl;

    // Monday is day of week zero

    for (int dow = 0; dow < 7; ++dow)
    {
        date += 1;

        clib::Weekday weekday = clib::DayOfWeek(date);
        clib::Weekday::Enum eWeekday = weekday;

        std::cout << date.ToString() << " : " << weekday << " : " << eWeekday << " : " << dow << std::endl;

        SPI_POST_CONDITION(eWeekday == dow);
    }
}

static void testDaysInMonth(void)
{
    int normalYear = 2014;
    int leapYear = 2016;
    for (int month = 1; month <= 12; ++month)
    {
        int normalDays = clib::DaysInMonth(normalYear, month);
        int leapDays   = clib::DaysInMonth(leapYear, month);

        if (month == 2)
        {
            SPI_POST_CONDITION(leapDays - normalDays == 1);
        }
        else
        {
            SPI_POST_CONDITION(leapDays == normalDays);
        }

        switch (month)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            SPI_POST_CONDITION(normalDays == 31);
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            SPI_POST_CONDITION(normalDays == 30);
            break;
        case 2:
            SPI_POST_CONDITION(normalDays == 28);
            break;
        default:
            throw spi::RuntimeError("PROGRAM_BUG");
        }
    }
}

static void testDateToEOM(void)
{
    for (int year = 2015; year <= 2016; ++year)
    {
        for (int month = 1; month <= 12; ++month)
        {
            int daysInMonth = clib::DaysInMonth(year, month);
            spi::Date date = clib::Date(year, month, 15);
            spi::Date endOfMonth;

            clib::DateToEOM(date, endOfMonth);

            SPI_POST_CONDITION(endOfMonth = clib::Date(year, month, daysInMonth));
        }
    }
}

static void testDateIntervalFreq(void)
{
    // DateInterval is a simple type and we convert freely to/from strings
    int frequencies[] = {12, 6, 4, 3, 2, 1};

    for (size_t i = 0; i < 6; ++i)
    {
        int frequency = frequencies[i];
        int months = 12 / frequency;
        std::string interval = spi_util::StringFormat("%dM", months);
        // note that DateIntervalConstSP can be constructed implicitly from std::string
        double freq = clib::DateIntervalFreq(interval);

        std::cout << interval << " " << freq << " " << frequency << std::endl;

        SPI_POST_CONDITION(freq == frequency);
    }
}

static void testDateIntervalYears(void)
{
    // DateInterval is a simple type and we convert freely to/from strings
    double years[] = {0.5, 1, 1.5, 2, 3, 4, 5};
    int months[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int weeks[] = {1, 2, 3};
    int days[] = {1, 2, 3, 7, 14};

    for (size_t i = 0; i < sizeof(years)/sizeof(double); ++i)
    {
        char interval[16];
        double year = years[i];
        if (year == (double)(int)year)
        {
            sprintf(interval, "%dY", (int)year);
        }
        else
        {
            sprintf(interval, "%dM", (int)(year*12));
        }
        // note that DateIntervalConstSP can be constructed implicitly from std::string
        // and hence from const char[16] as well
        double result = clib::DateIntervalYears(interval);

        std::cout << interval << " " << result << std::endl;

        SPI_POST_CONDITION(result == year);
    }

    for (size_t i = 0; i < sizeof(months)/sizeof(int); ++i)
    {
        char interval[16];
        int month = months[i];
        sprintf(interval, "%dM", month);

        double result = clib::DateIntervalYears(interval);

        std::cout << interval << " " << result << std::endl;

        SPI_POST_CONDITION(result == month/12.0);
    }

    for (size_t i = 0; i < sizeof(weeks)/sizeof(int); ++i)
    {
        char interval[16];
        int week = weeks[i];
        sprintf(interval, "%dW", week);
        double result = clib::DateIntervalYears(interval);

        std::cout << interval << " " << result << std::endl;

        SPI_POST_CONDITION(result == 7*week/365.0);
    }

    for (size_t i = 0; i < sizeof(days)/sizeof(int); ++i)
    {
        char interval[16];
        int day = days[i];
        sprintf(interval, "%dD", day);

        double result = clib::DateIntervalYears(interval);

        std::cout << interval << " " << result << std::endl;

        SPI_POST_CONDITION(result == day / 365.0);
    }
}

int main(int argc, char* argv[])
{
    clib::clib_exported_service()->start_logging("dates.log", "");
    return spi::TestRunner(argc, argv,
                           "DayOfWeek", testDayOfWeek,
                           "DaysInMonth", testDaysInMonth,
                           "DateToEOM", testDateToEOM,
                           "DateIntervalFreq", testDateIntervalFreq,
                           "DateIntervalYears", testDateIntervalYears,
                           NULL);
}
