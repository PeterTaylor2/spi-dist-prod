
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

#include "DiscountCurve.hpp"

#include <utils/RuntimeError.hpp>

CDiscountCurve::CDiscountCurve(
    CDate anchorDate,
    const IZeroCurveConstSP& zc)
    :
    m_anchorDate(anchorDate),
    m_zc(zc)
{
    VerifyAndComplete();
}

void CDiscountCurve::VerifyAndComplete()
{
    PRE_CONDITION(m_anchorDate);
    PRE_CONDITION(m_zc);

    m_anchorDiscount = m_zc->PV(m_anchorDate);
}

double CDiscountCurve::Discount(CDate date) const
{
    try
    {
        double dateDiscount = m_zc->PV(date);
        double discount = dateDiscount / m_anchorDiscount;

        return discount;
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}

double CDiscountCurve::FwdDiscount(CDate startDate, CDate endDate) const
{
    try
    {
        double startDiscount = m_zc->PV(startDate);
        double endDiscount   = m_zc->PV(endDate);
        double discount = endDiscount / startDiscount;

        return discount;
    }
    catch (std::exception& e)
    {
        THROW_FUNCTION_FAILURE(e);
    }
}
