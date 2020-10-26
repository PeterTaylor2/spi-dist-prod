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
** DayCount.cpp
***************************************************************************
** DayCount class for calculating the differences between dates.
** We make great use of singleton classes.
***************************************************************************
*/

#include "DayCount.hpp"

#include "EnumLookup.hpp"
#include "RuntimeError.hpp"

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_ANONYMOUS_NAMESPACE

class null_deleter
{
public:
    void operator()(const void* p)
    {}
};

template<class T>
boost::shared_ptr<const T> share_singleton(const T* singleton)
{
    return boost::shared_ptr<const T>(singleton, null_deleter());
}


CEnumLookup* StandardDayCountEnumLookup()
{
    static CEnumLookup lookup("DayCount");
    static bool init = false;

    if (!init)
    {
        lookup.AddItem(1, "ACT/365F");
        lookup.AddItem(1, "ACTUAL/365F");
        lookup.AddItem(1, "A/365F");
        lookup.AddItem(1, "ACT_365F");

        lookup.AddItem(2, "ACT/360");
        lookup.AddItem(2, "ACTUAL/360");
        lookup.AddItem(2, "A/360");
        lookup.AddItem(2, "ACT_360");

        lookup.AddItem(3, "ACT/ACT");
        lookup.AddItem(3, "ACTUAL/ACTUAL");
        lookup.AddItem(3, "A/A");
        lookup.AddItem(3, "ACT_ACT");

        lookup.AddItem(4, "B30/360");
        lookup.AddItem(4, "30/360");
        lookup.AddItem(4, "B30_360");

        lookup.AddItem(5, "B30E/360");
        lookup.AddItem(5, "30E/360");
        lookup.AddItem(5, "B30E_360");

        init = true;
    }

    return &lookup;
}

int StandardDayCountFromString(const char* str)
{
    int code = StandardDayCountEnumLookup()->FromString(str);
    return code;
}

END_ANONYMOUS_NAMESPACE

/*
**************************************************************************
** Implementation of base class CDayCount
**************************************************************************
*/
CDayCountConstSP CDayCount::FromString(const std::string& str)
{
    return CDayCountStandard::Make(str);
}

CDayCountActual365FConstSP CDayCount::ACT_365F = CDayCountActual365F::Make();
CDayCountActual360ConstSP CDayCount::ACT_360 = CDayCountActual360::Make();
CDayCountActualActualConstSP CDayCount::ACT_ACT = CDayCountActualActual::Make();
CDayCountB30360ConstSP CDayCount::B30_360 = CDayCountB30360::Make();
CDayCountB30E360ConstSP CDayCount::B30E_360 = CDayCountB30E360::Make();

CDayCount::~CDayCount()
{}

/*
**************************************************************************
** Implementation of intermediate base class CDayCountStandard
**************************************************************************
*/
CDayCountStandardConstSP CDayCountStandard::Make(const std::string& str)
{
    int code = StandardDayCountFromString(str.c_str());
    switch (code)
    {
    case 1:
        return CDayCountActual365F::Make();
    case 2:
        return CDayCountActual360::Make();
    case 3:
        return CDayCountActualActual::Make();
    case 4:
        return CDayCountB30360::Make();
    case 5:
        return CDayCountB30E360::Make();
    }

    THROW_RUNTIME_ERROR("Unknown code " << code);
}

/*
**************************************************************************
** Implementation of ACT/365F
**************************************************************************
*/
CDayCountActual365F::CDayCountActual365F()
{}

CDayCountActual365FConstSP CDayCountActual365F::Make()
{
    static CDayCountActual365F singleton;
    return share_singleton(&singleton);
}

double CDayCountActual365F::Time(CDate startDate, CDate endDate) const
{
    return Days(startDate, endDate) / 365.0;
}

int CDayCountActual365F::Days(CDate startDate, CDate endDate) const
{
    return endDate - startDate;
}

std::string CDayCountActual365F::Name() const
{
    return "ACT/365F";
}

/*
**************************************************************************
** Implementation of ACT/360
**************************************************************************
*/
CDayCountActual360::CDayCountActual360()
{}

CDayCountActual360ConstSP CDayCountActual360::Make()
{
    static CDayCountActual360 singleton;
    return share_singleton(&singleton);
}

double CDayCountActual360::Time(CDate startDate, CDate endDate) const
{
    return Days(startDate, endDate) / 360.0;
}

int CDayCountActual360::Days(CDate startDate, CDate endDate) const
{
    return endDate - startDate;
}

std::string CDayCountActual360::Name() const
{
    return "ACT/360";
}

/*
**************************************************************************
** Implementation of ACT/ACT
**************************************************************************
*/
CDayCountActualActual::CDayCountActualActual()
{}

CDayCountActualActualConstSP CDayCountActualActual::Make()
{
    static CDayCountActualActual singleton;
    return share_singleton(&singleton);
}

double CDayCountActualActual::Time(CDate startDate, CDate endDate) const
{
    if (startDate == endDate)
        return 0.0;

    double sign = 1.0;
    if (startDate > endDate)
    {
        sign = -1.0;
        std::swap(startDate, endDate);
    }

    int Y1, M1, D1;
    int Y2, M2, D2;
    
    startDate.YMD(&Y1, &M1, &D1);
    endDate.YMD(&Y2, &M2, &D2);

    double time = 0.0;
    if (Y2 > Y1)
    {
        // the assumption is that we include startDate and exclude endDate
        time += (Y2-Y1-1);
        int days1 = CDate(Y1+1,1,1) - startDate;
        int days2 = endDate - CDate(Y2,1,1);

        if (IsLeapYear(Y1))
            time += days1 / 366.0;
        else
            time += days1 / 365.0;

        if (IsLeapYear(Y2))
            time += days2 / 366.0;
        else
            time += days2 / 365.0;
    }
    else
    {
        int days = endDate - startDate;
        if (IsLeapYear(Y1))
            time = days / 366.0;
        else
            time = days / 365.0;
    }

    return sign * time;
}

int CDayCountActualActual::Days(CDate startDate, CDate endDate) const
{
    return endDate - startDate;
}

std::string CDayCountActualActual::Name() const
{
    return "ACT/ACT";
}

/*
**************************************************************************
** Implementation of B30/360
**************************************************************************
*/
CDayCountB30360::CDayCountB30360()
{}

CDayCountB30360ConstSP CDayCountB30360::Make()
{
    static CDayCountB30360 singleton;
    return share_singleton(&singleton);
}

double CDayCountB30360::Time(CDate startDate, CDate endDate) const
{
    return Days(startDate, endDate) / 360.0;
}

int CDayCountB30360::Days(CDate startDate, CDate endDate) const
{
    if (startDate == endDate)
        return 0;

    int sign = 1;
    if (startDate > endDate)
    {
        sign = -1;
        std::swap(startDate, endDate);
    }

    int Y1, M1, D1;
    int Y2, M2, D2;
    
    startDate.YMD(&Y1, &M1, &D1);
    endDate.YMD(&Y2, &M2, &D2);

    if (D1 == 31)
        D1 = 30;

    if (D2 == 31 && D1 == 30)
        D2 = 30;

    int days = (Y2-Y1)*360 + (M2-M1)*30 + (D2-D1);

    return sign * days;
}

std::string CDayCountB30360::Name() const
{
    return "B30/360";
}

/*
**************************************************************************
** Implementation of B30E/360
**************************************************************************
*/
CDayCountB30E360::CDayCountB30E360()
{}

CDayCountB30E360ConstSP CDayCountB30E360::Make()
{
    static CDayCountB30E360 singleton;
    return share_singleton(&singleton);
}

double CDayCountB30E360::Time(CDate startDate, CDate endDate) const
{
    return Days(startDate, endDate) / 360.0;
}

int CDayCountB30E360::Days(CDate startDate, CDate endDate) const
{
    if (startDate == endDate)
        return 0;

    int sign = 1;
    if (startDate > endDate)
    {
        sign = -1;
        std::swap(startDate, endDate);
    }

    int Y1, M1, D1;
    int Y2, M2, D2;
    
    startDate.YMD(&Y1, &M1, &D1);
    endDate.YMD(&Y2, &M2, &D2);

    if (D1 == 31)
        D1 = 30;

    // slight difference here from B30/360 - we don't wait for D1=30
    // before turning 31st into 30th
    if (D2 == 31)
        D2 = 30;

    int days = (Y2-Y1)*360 + (M2-M1)*30 + (D2-D1);

    return sign * days;
}

std::string CDayCountB30E360::Name() const
{
    return "B30E/360";
}
