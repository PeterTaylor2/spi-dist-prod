#ifndef CPPLIB_DISCOUNT_CURVE_HPP
#define CPPLIB_DISCOUNT_CURVE_HPP

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

#include "ZeroCurve.hpp"

CPPLIB_DECLARE_CLASS(CDiscountCurve);

class CDiscountCurve
{
public:
    CDiscountCurve(CDate anchorDate, const IZeroCurveConstSP& zc);

    CDate AnchorDate() const;
    const IZeroCurveConstSP& ZeroCurve() const;

    double Discount(CDate date) const;
    double FwdDiscount(CDate startDate, CDate endDate) const;

private:
    CDate m_anchorDate;
    IZeroCurveConstSP m_zc;

    double m_anchorDiscount;
    void VerifyAndComplete();
};

#endif
