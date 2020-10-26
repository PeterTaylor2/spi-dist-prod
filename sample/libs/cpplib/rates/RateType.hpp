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
#ifndef CPPLIB_RATE_TYPE_HPP
#define CPPLIB_RATE_TYPE_HPP

/**
***************************************************************************
** RateType.hpp
***************************************************************************
** RateType class defines how a rate is quoted.
***************************************************************************
*/

#include <utils/Declare.hpp>
#include <utils/Date.hpp>
#include <utils/DayCount.hpp>

#include <string>

struct CRateType
{
    CRateType(int number);
    CRateType(const char* name);
    CRateType(const std::string& name);
    CRateType(double freq);

    static const int SIMPLE = 0;
    static const int ANNUAL = 1;
    static const int SEMI_ANNUAL = 2;
    static const int QUARTERLY = 4;
    static const int MONTHLY = 12;
    static const int DAILY = 365;
    static const int CONTINUOUS = 5000;
    static const int DISCOUNT_RATE = 512;
    static const int DISCOUNT_FACTOR = -2;

    int number;
    std::string Name();

    double RateToDiscount(double rate, double time) const;
    double DiscountToRate(double discount, double time) const;
};

double RateToDiscount(
    double rate,
    CDate startDate,
    CDate maturityDate,
    CDayCountConstSP dayCount,
    CRateType rateType);

double DiscountToRate(
    double discount,
    CDate startDate,
    CDate maturityDate,
    CDayCountConstSP dayCount,
    CRateType rateType);

#endif


