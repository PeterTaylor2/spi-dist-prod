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
#ifndef CPPLIB_ZERO_CURVE_HPP
#define CPPLIB_ZERO_CURVE_HPP

/**
***************************************************************************
** ZeroCurve.hpp
***************************************************************************
** Defines a curve of zero coupon rates which can subsequently be used 
** for a discount curve or forward rate curve.
***************************************************************************
*/

#include <utils/Declare.hpp>
#include <utils/Date.hpp>
#include <utils/DayCount.hpp>

#include "RateType.hpp"

#include <string>
#include <vector>

CPPLIB_DECLARE_CLASS(IZeroCurve);
CPPLIB_DECLARE_CLASS(CZeroCurve);
CPPLIB_DECLARE_CLASS(CCashFlowList);
CPPLIB_DECLARE_CLASS(CZeroCurveProduct);

class IZeroCurve
{
public:

    virtual ~IZeroCurve();

    virtual CDate BaseDate() const = 0;
    virtual double PV(CDate interpDate) const = 0;
    virtual CDate FirstDate() const = 0;
};


class CZeroCurve : public IZeroCurve
{
public:

    enum EInterpType
    {
        LINEAR,
        FLAT_FORWARDS
    };

    CZeroCurve(
        CDate baseDate,
        EInterpType interpType,
        const std::vector<CDate>& dates,
        const std::vector<double>& rates,
        CRateType rateType,
        const CDayCountConstSP& dayCount);
        
    CDate BaseDate() const;
    EInterpType InterpType() const;
    const std::vector<CDate>& Dates() const;
    const std::vector<double>& Rates() const;
    CRateType RateType() const;
    const CDayCountConstSP& DayCount() const;

    double ZeroRate(CDate interpDate) const;
    double PV(CDate interpDate) const;
    CDate FirstDate() const;
    CDate LastDate() const;

private:
    CDate m_baseDate;
    EInterpType m_interpType;
    std::vector<CDate> m_dates;
    std::vector<double> m_rates;
    CRateType m_rateType;
    CDayCountConstSP m_dayCount;

    void VerifyAndComplete();

    double LinearZeroRate(CDate interpDate) const;
    double FlatForwardsPV(CDate interpDate) const;
};

class CZeroCurveProduct : public IZeroCurve
{
public:

    CZeroCurveProduct(
        CDate baseDate,
        const IZeroCurveConstSP& zc1,
        const IZeroCurveConstSP& zc2,
        double baseDateDiscount = 0.0);

    CDate BaseDate() const;
    double PV(CDate interpDate) const;
    CDate FirstDate() const;
    double BaseDateDiscount() const;
    const IZeroCurveConstSP ZC1() const;
    const IZeroCurveConstSP ZC2() const;

private:
    CDate m_baseDate;
    IZeroCurveConstSP m_zc1;
    IZeroCurveConstSP m_zc2;
    double m_baseDateDiscount;

    void VerifyAndComplete();
};

double ForwardRate(
    const IZeroCurveConstSP& zeroCurve,
    CDate startDate,
    CDate maturityDate,
    const CDayCountConstSP& dcc,
    CRateType rateType);

double FlowsPV(
    const CCashFlowListConstSP& cfl,
    const IZeroCurveConstSP& zc,
    CDate cutoffDate=CDate());

double FlowsFV(
    const CCashFlowListConstSP& cfl,
    const IZeroCurveConstSP& zc,
    CDate valueDate,
    CDate cutoffDate=CDate());

#endif


