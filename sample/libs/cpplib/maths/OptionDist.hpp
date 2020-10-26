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

#ifndef CPPLIB_OPTION_DIST_HPP
#define CPPLIB_OPTION_DIST_HPP

/**
***************************************************************************
** OptionDist.hpp
***************************************************************************
** Defines the COptionDist and sub-classes which can be used to provide
** the analytics for a model which depends purely on the terminal
** distribution of the underlying.
**
** Results are given in the same units as the forward and it is expected
** that the caller will convert these to PV using the relevant measure.
***************************************************************************
*/

#include <utils/Declare.hpp>

#include <string>
#include <vector>

CPPLIB_DECLARE_CLASS(COptionDist);
CPPLIB_DECLARE_CLASS(COptionLognormal);
CPPLIB_DECLARE_CLASS(COptionNormal);

enum ECallOrPut : char
{
    OPTION_CALL = 'C',
    OPTION_PUT  = 'P'
};

class COptionPremiumFunction;

class COptionDist
{
public:
    friend class COptionPremiumFunction;

    virtual ~COptionDist() {}

    virtual double CallPremium(
        double fwd, double expiry, double strike) const = 0;
    virtual double CallDelta(
        double fwd, double expiry, double strike) const = 0;

    virtual double PutPremium(
        double fwd, double expiry, double strike) const;
    double Premium(
        ECallOrPut callOrPut,
        double fwd, double expiry, double strike) const;
    virtual double PutDelta(
        double fwd, double expiry, double strike) const;
    double Delta(
        ECallOrPut callOrPut,
        double fwd, double expiry, double strike) const;

    virtual double Volatility() const = 0;
    double ImpliedVolatility(
        ECallOrPut callOrPut,
        double fwd, double expiry, double strike, double premium) const;

protected:
    virtual void SetVolatility(double volatility) = 0;
    virtual COptionDist* Clone() const = 0;
};



class COptionLognormal : public COptionDist
{
public:

    COptionLognormal(double vol);

    double CallPremium(
        double fwd, double expiry, double strike) const;
    double CallDelta(
        double fwd, double expiry, double strike) const;

    double Volatility() const;

protected:
    void SetVolatility(double volatility);
    COptionDist* Clone() const;

private:
    double m_vol;
};

class COptionNormal : public COptionDist
{
public:

    COptionNormal(double vol);

    double CallPremium(
        double fwd, double expiry, double strike) const;
    double CallDelta(
        double fwd, double expiry, double strike) const;

    double Volatility() const;

protected:
    void SetVolatility(double volatility);
    COptionDist* Clone() const;

private:
    double m_vol;
};

#endif
