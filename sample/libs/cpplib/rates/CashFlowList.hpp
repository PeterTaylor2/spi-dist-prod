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
#ifndef CPPLIB_CASH_FLOW_LIST_HPP
#define CPPLIB_CASH_FLOW_LIST_HPP

/**
***************************************************************************
** CashFlowList.hpp
***************************************************************************
** Defines a list of cash flows.
***************************************************************************
*/

#include <utils/Declare.hpp>
#include <utils/Date.hpp>

#include <vector>

CPPLIB_DECLARE_CLASS(CCashFlowList);

class CCashFlowList
{
public:

    static const char* TypeName;

    CCashFlowList(const std::vector<CDate>& dates,
        const std::vector<double>& amounts);

    const std::vector<CDate>& Dates() const;
    const std::vector<double>& Amounts() const;
    int NumItems() const;

    static CCashFlowListConstSP Merge(
        const CCashFlowListConstSP& a,
        const CCashFlowListConstSP& b);

private:

    std::vector<CDate> m_dates;
    std::vector<double> m_amounts;

    void VerifyAndComplete();
};


#endif


