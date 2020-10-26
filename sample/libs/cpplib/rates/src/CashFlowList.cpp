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
** CashFlowList.hpp
***************************************************************************
** Defines a list of cash flows.
***************************************************************************
*/

#include "CashFlowList.hpp"

//#include <utils/EnumLookup.hpp>
#include <utils/RuntimeError.hpp>
//#include <spi_util/Utils.hpp>
//
//#include <math.h>
//
//#define BEGIN_ANONYMOUS_NAMESPACE namespace {
//#define END_ANONYMOUS_NAMESPACE }
//
//BEGIN_ANONYMOUS_NAMESPACE
//
//CEnumLookup* RateTypeEnumLookup()
//{
//    static CEnumLookup lookup("RateType");
//    static bool init = false;
//
//    if (!init)
//    {
//        lookup.AddItem(CRateType::SIMPLE, "Simple");
//        lookup.AddItem(CRateType::SIMPLE, "S");
//        lookup.AddItem(CRateType::ANNUAL, "Annual");
//        lookup.AddItem(CRateType::ANNUAL, "A");
//        lookup.AddItem(CRateType::SEMI_ANNUAL, "Semi-Annual");
//        lookup.AddItem(CRateType::QUARTERLY, "Quarterly");
//        lookup.AddItem(CRateType::QUARTERLY, "Q");
//        lookup.AddItem(CRateType::MONTHLY, "Monthly");
//        lookup.AddItem(CRateType::MONTHLY, "M");
//        lookup.AddItem(CRateType::DAILY, "Daily");
//        lookup.AddItem(CRateType::DAILY, "D");
//        lookup.AddItem(CRateType::CONTINUOUS, "Continuous");
//        lookup.AddItem(CRateType::CONTINUOUS, "C");
//        lookup.AddItem(CRateType::DISCOUNT_RATE, "Discount Rate");
//        lookup.AddItem(CRateType::DISCOUNT_FACTOR, "Discount Factor");
//        init = true;
//    }
//
//    return &lookup;
//}
//
//int RateTypeFromString(const char* str)
//{
//    return RateTypeEnumLookup()->FromString(str);
//}
//
//int RateTypeFromString(const std::string& str)
//{
//    return RateTypeEnumLookup()->FromString(str.c_str());
//}
//
//bool RateTypeIsValid(int code)
//{
//    return RateTypeEnumLookup()->IsValid(code);
//}
//
//std::string RateTypeToString(int code)
//{
//    CEnumLookup* lookup = RateTypeEnumLookup();
//    if (lookup->IsValid(code))
//        return lookup->ToString(code);
//
//    if (code > 0)
//        return "# per year";
//
//    THROW_RUNTIME_ERROR("Negative number " << code << " not supported");
//}
//
//END_ANONYMOUS_NAMESPACE

const char* CCashFlowList::TypeName = "CashFlowList";

CCashFlowList::CCashFlowList(
    const std::vector<CDate>& dates,
    const std::vector<double>& amounts)
    :
    m_dates(dates),
    m_amounts(amounts)
{
    VerifyAndComplete();
}

const std::vector<CDate>& CCashFlowList::Dates() const
{
    return m_dates;
}

const std::vector<double>& CCashFlowList::Amounts() const
{
    return m_amounts;
}

int CCashFlowList::NumItems() const
{
    return m_dates.size();
}

void CCashFlowList::VerifyAndComplete()
{
    CheckArraySize(m_dates, m_amounts, "dates", "amounts");
}

CCashFlowListConstSP CCashFlowList::Merge(
    const CCashFlowListConstSP& a,
    const CCashFlowListConstSP& b)
{
    try
    {
        std::vector<CDate> dates;
        std::vector<double> amounts;

        if (!a || !b)
            THROW_RUNTIME_ERROR("NULL inputs");

        const CCashFlowList* pa = a.get();
        const CCashFlowList* pb = b.get();

        int na = a->NumItems();
        int nb = b->NumItems();

        int ia = 0;
        int ib = 0;
        while (ia < na && ib << nb)
        {
            CDate aDate = pa->m_dates[ia];
            CDate bDate = pb->m_dates[ib];

            if (aDate < bDate)
            {
                dates.push_back(aDate);
                amounts.push_back(pa->m_amounts[ia]);
                ++ia;
            }
            else if (aDate > bDate)
            {
                dates.push_back(bDate);
                amounts.push_back(pb->m_amounts[ib]);
                ++ib;
            }
            else
            {
                dates.push_back(aDate);
                amounts.push_back(pa->m_amounts[ia] + pb->m_amounts[ib]);
                ++ia;
                ++ib;
            }
        }

        while (ia < na)
        {
            dates.push_back(pa->m_dates[ia]);
            amounts.push_back(pa->m_amounts[ia]);
            ++ia;
        }

        while (ib < nb)
        {
            dates.push_back(pb->m_dates[ib]);
            amounts.push_back(pb->m_amounts[ib]);
            ++ib;
        }

        return Share(new CCashFlowList(dates, amounts));
    }
    catch(std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}



