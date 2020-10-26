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
** RateType.cpp
***************************************************************************
** RateType class defines how a rate is quoted.
***************************************************************************
*/

#include "RateType.hpp"

#include <utils/EnumLookup.hpp>
#include <utils/RuntimeError.hpp>
#include <spi_util/Utils.hpp>

#include <math.h>

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_ANONYMOUS_NAMESPACE

CEnumLookup* RateTypeEnumLookup()
{
    static CEnumLookup lookup("RateType");
    static bool init = false;

    if (!init)
    {
        lookup.AddItem(CRateType::SIMPLE, "Simple");
        lookup.AddItem(CRateType::SIMPLE, "S");
        lookup.AddItem(CRateType::ANNUAL, "Annual");
        lookup.AddItem(CRateType::ANNUAL, "A");
        lookup.AddItem(CRateType::SEMI_ANNUAL, "Semi-Annual");
        lookup.AddItem(CRateType::QUARTERLY, "Quarterly");
        lookup.AddItem(CRateType::QUARTERLY, "Q");
        lookup.AddItem(CRateType::MONTHLY, "Monthly");
        lookup.AddItem(CRateType::MONTHLY, "M");
        lookup.AddItem(CRateType::DAILY, "Daily");
        lookup.AddItem(CRateType::DAILY, "D");
        lookup.AddItem(CRateType::CONTINUOUS, "Continuous");
        lookup.AddItem(CRateType::CONTINUOUS, "C");
        lookup.AddItem(CRateType::DISCOUNT_RATE, "Discount Rate");
        lookup.AddItem(CRateType::DISCOUNT_FACTOR, "Discount Factor");
        init = true;
    }

    return &lookup;
}

int RateTypeFromString(const char* str)
{
    return RateTypeEnumLookup()->FromString(str);
}

int RateTypeFromString(const std::string& str)
{
    return RateTypeEnumLookup()->FromString(str.c_str());
}

std::string RateTypeToString(int code)
{
    CEnumLookup* lookup = RateTypeEnumLookup();
    if (lookup->IsValid(code))
        return lookup->ToString(code);

    if (code > 0)
        return "# per year";

    THROW_RUNTIME_ERROR("Negative number " << code << " not supported");
}

END_ANONYMOUS_NAMESPACE

CRateType::CRateType(int number) : number(number)
{}

CRateType::CRateType(double freq) : number()
{
    try
    {
        number = spi_util::IntegerCast<int>(freq);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

CRateType::CRateType(const char* name) : number()
{
    try
    {
        number = RateTypeFromString(name);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

CRateType::CRateType(const std::string& name) : number()
{
    try
    {
        number = RateTypeFromString(name);
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

std::string CRateType::Name()
{
    return RateTypeToString(number);
}

double CRateType::RateToDiscount(double rate, double time) const
{
    if (number == DISCOUNT_FACTOR)
        return rate;

    if (time == 0.0)
        return 1.0;

    switch (number)
    {
    case SIMPLE:
        {
            double denom = 1.0 + rate * time;
            if (denom <= 0.0)
                THROW_RUNTIME_ERROR("Invalid simple interest rate " << rate << " for time " << time);
            return 1.0/denom;
        }
    case CONTINUOUS:
        return exp(-rate*time);
    case DISCOUNT_RATE:
        {
            double discount = 1.0 - rate* time;
            if (discount <= 0.0)
                THROW_RUNTIME_ERROR("Invalid discount rate " << rate << " for time " << time);
            return discount;
        }
    default:
        break;
    }

    if (number < 0)
        THROW_RUNTIME_ERROR("Negative number " << number << " not supported");

    double tmp = 1.0 + rate / number;

    if (tmp <= 0.0)
        THROW_RUNTIME_ERROR("Bad rate " << rate << " for time " << time << " and frequency " << number);

    return pow(tmp, -number*time);
}

double CRateType::DiscountToRate(double discount, double time) const
{
    PRE_CONDITION(discount > 0.0);

    if (number == DISCOUNT_FACTOR)
        return discount;

    PRE_CONDITION(time != 0.0);

    switch (number)
    {
    case SIMPLE:
        return (1.0/discount - 1.0)/time;
    case CONTINUOUS:
        return -log(discount)/time;
    case DISCOUNT_RATE:
        return (1.0 - discount)/time;
    default:
        break;
    }

    if (number < 0)
        THROW_RUNTIME_ERROR("Negative number " << number << " not supported");
    return number * (pow(discount, -1.0/(number*time)) - 1.0);
}

double RateToDiscount(
    double rate,
    CDate startDate,
    CDate maturityDate,
    CDayCountConstSP dayCount,
    CRateType rateType)
{
    if (rateType.number == CRateType::DISCOUNT_FACTOR)
        return rate;

    double time = dayCount->Time(startDate, maturityDate);
    return rateType.RateToDiscount(rate, time);
}

double DiscountToRate(
    double discount,
    CDate startDate,
    CDate maturityDate,
    CDayCountConstSP dayCount,
    CRateType rateType)
{
    PRE_CONDITION(discount > 0.0);

    if (rateType.number == CRateType::DISCOUNT_FACTOR)
        return discount;

    PRE_CONDITION(startDate != maturityDate);
    double time = dayCount->Time(startDate, maturityDate);
    return rateType.DiscountToRate(discount, time);
}


