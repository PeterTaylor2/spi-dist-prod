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
#ifndef CPPLIB_DAY_COUNT_HPP
#define CPPLIB_DAY_COUNT_HPP

/**
***************************************************************************
** DayCount.hpp
***************************************************************************
** DayCount class for calculating the differences between dates.
** We make great use of singleton classes.
***************************************************************************
*/

#include "Date.hpp"
#include "Declare.hpp"

#include <string>

CPPLIB_DECLARE_CLASS(CDayCount);
CPPLIB_DECLARE_CLASS(CDayCountStandard);
CPPLIB_DECLARE_CLASS(CDayCountActual365F);
CPPLIB_DECLARE_CLASS(CDayCountActual360);
CPPLIB_DECLARE_CLASS(CDayCountActualActual);
CPPLIB_DECLARE_CLASS(CDayCountB30360);
CPPLIB_DECLARE_CLASS(CDayCountB30E360);

class CDayCount
{
public:
    virtual ~CDayCount();
    virtual double Time(CDate startDate, CDate endDate) const = 0;
    virtual int Days(CDate startDate, CDate endDate) const = 0;
    static CDayCountConstSP FromString(const std::string& str);
    static CDayCountActual365FConstSP ACT_365F;
    static CDayCountActual360ConstSP ACT_360;
    static CDayCountActualActualConstSP ACT_ACT;
    static CDayCountB30360ConstSP B30_360;
    static CDayCountB30E360ConstSP B30E_360;
};

// The intention is to also introduce CDayCountBusinessDays or such-like
// The standard day count conventions can all be constructed from a string
// Also we define constants within the the main CDayCount class for all of
// the standard day count conventions, so you can say something like
// CDayCount::ACT_365F to represent a particular standard day count convention
class CDayCountStandard : public CDayCount
{
public:
    static CDayCountStandardConstSP Make(const std::string& str);
    virtual std::string Name() const = 0;
};

class CDayCountActual365F : public CDayCountStandard
{
public:
    static CDayCountActual365FConstSP Make();
    double Time(CDate startDate, CDate endDate) const;
    int Days(CDate startDate, CDate endDate) const;
    std::string Name() const;
private:
    CDayCountActual365F();
};

class CDayCountActual360 : public CDayCountStandard
{
public:
    static CDayCountActual360ConstSP Make();
    double Time(CDate startDate, CDate endDate) const;
    int Days(CDate startDate, CDate endDate) const;
    std::string Name() const;
private:
    CDayCountActual360();
};

class CDayCountActualActual : public CDayCountStandard
{
public:
    static CDayCountActualActualConstSP Make();
    double Time(CDate startDate, CDate endDate) const;
    int Days(CDate startDate, CDate endDate) const;
    std::string Name() const;
private:
    CDayCountActualActual();
};

class CDayCountB30360 : public CDayCountStandard
{
public:
    static CDayCountB30360ConstSP Make();
    double Time(CDate startDate, CDate endDate) const;
    int Days(CDate startDate, CDate endDate) const;
    std::string Name() const;
private:
    CDayCountB30360();
};

class CDayCountB30E360 : public CDayCountStandard
{
public:
    static CDayCountB30E360ConstSP Make();
    double Time(CDate startDate, CDate endDate) const;
    int Days(CDate startDate, CDate endDate) const;
    std::string Name() const;
private:
    CDayCountB30E360();
};

#endif


