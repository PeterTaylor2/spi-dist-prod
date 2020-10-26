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
** OptionDist.cpp
***************************************************************************
** Defines the COptionDist and sub-classes which can be used to provide
** the analytics for a model which depends purely on the terminal
** distribution of the underlying.
**
** Results are given in the same units as the forward and it is expected
** that the caller will convert these to PV using the relevant measure.
***************************************************************************
*/

#include "OptionDist.hpp"
#include "NormDist.hpp"

#include <utils/RuntimeError.hpp>
#include <utils/RootBrent.hpp>

#include <math.h>

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

BEGIN_ANONYMOUS_NAMESPACE

bool IsPut(ECallOrPut callOrPut)
{
    switch(callOrPut)
    {
    case OPTION_CALL: return false;
    case OPTION_PUT: return true;
    }

    THROW_RUNTIME_ERROR("Neither call nor put");
}

#define IsCall(x) !IsPut(x)

END_ANONYMOUS_NAMESPACE

/**
***************************************************************************
** Declaration and implementation of COptionPremiumFunction
**
** Note that COptionPremiumFunction is a friend of COptionDist and hence
** is able to call the protected method SetVolatility.
***************************************************************************
*/
class COptionPremiumFunction : public CFunction1
{
public:
    COptionPremiumFunction(const COptionDistSP& optionDist,
               ECallOrPut callOrPut,
               double fwd,
               double expiry,
               double strike)
    :
    m_optionDist(optionDist),
    m_callOrPut(callOrPut),
    m_fwd(fwd),
    m_expiry(expiry),
    m_strike(strike)
    {
    }

    double operator()(double vol) const
    {
        m_optionDist->SetVolatility(vol);
        return m_optionDist->Premium(m_callOrPut, m_fwd, m_expiry, m_strike);
    }

private:
    COptionDistSP m_optionDist;
    ECallOrPut    m_callOrPut;
    double        m_fwd;
    double        m_expiry;
    double        m_strike;
};

/**
***************************************************************************
** Implementation of COptionDist
***************************************************************************
*/

double COptionDist::PutPremium(
    double fwd, double expiry, double strike) const
{
    double callPremium = CallPremium(fwd, expiry, strike);
    double putPremium = callPremium - fwd + strike;

    return putPremium;
}

double COptionDist::Premium(
    ECallOrPut callOrPut,
    double fwd, double expiry, double strike) const
{
    double premium;
    if (IsPut(callOrPut))
        premium = PutPremium(fwd, expiry, strike);
    else
        premium = CallPremium(fwd, expiry, strike);

    return premium;
}

double COptionDist::PutDelta(
    double fwd, double expiry, double strike) const
{
    double callDelta = CallDelta(fwd, expiry, strike);
    double putDelta  = callDelta - 1.0;

    return putDelta;
}

double COptionDist::Delta(
    ECallOrPut callOrPut,
    double fwd, double expiry, double strike) const
{
    double delta;
    if (IsPut(callOrPut))
        delta = PutDelta(fwd, expiry, strike);
    else
        delta = CallDelta(fwd, expiry, strike);

    return delta;
}


double COptionDist::ImpliedVolatility(
    ECallOrPut callOrPut,
    double fwd, double expiry, double strike, double premium) const
{
    const double boundLo = 0.0;
    const int numIterations = 100;
    const double xAccuracy = 0.1;
    const double fAccuracy = 1e-10 * premium;

    double guess = Volatility();
    double boundHi = std::max(2*guess, 100.0);

    COptionPremiumFunction func(
    COptionDistSP(Clone()),
        callOrPut,
        fwd,
        expiry,
        strike);

    double impliedVol = RootFindBrent(
        func,
        premium,
        boundLo,
        boundHi,
        numIterations,
        guess,
        xAccuracy,
        fAccuracy);

    return impliedVol;
}

/**
***************************************************************************
** Implementation of COptionLognormal
***************************************************************************
*/

COptionLognormal::COptionLognormal(double vol)
    :
    m_vol(vol)
{
    PRE_CONDITION(vol >= 0.0);
}

double COptionLognormal::CallPremium(
    double fwd, double expiry, double strike) const
{
    PRE_CONDITION(fwd > 0.0);
    PRE_CONDITION(expiry > 0.0);
    PRE_CONDITION(strike > 0.0);

    double volSqrtExpiry = m_vol * sqrt(expiry);

    double d1 = (log(fwd/strike) + m_vol*m_vol*expiry/2.0) / volSqrtExpiry;
    double d2 = d1 - volSqrtExpiry;

    double premium = fwd * NormDist::CND(d1) - strike * NormDist::CND(d2);

    return premium;
}

double COptionLognormal::CallDelta(
    double fwd, double expiry, double strike) const
{
    PRE_CONDITION(fwd > 0.0);
    PRE_CONDITION(expiry > 0.0);
    PRE_CONDITION(strike > 0.0);

    double volSqrtExpiry = m_vol * sqrt(expiry);
    double d1 = (log(fwd/strike) + m_vol*m_vol*expiry/2.0) / volSqrtExpiry;
    double delta = NormDist::CND(d1);

    return delta;
}

double COptionLognormal::Volatility() const
{
    return m_vol;
}

void COptionLognormal::SetVolatility(double volatility)
{
    PRE_CONDITION(volatility >= 0.0);
    m_vol = volatility;
}

COptionDist* COptionLognormal::Clone() const
{
    return new COptionLognormal(m_vol);
}


/**
***************************************************************************
** Implementation of COptionNormal
***************************************************************************
*/

COptionNormal::COptionNormal(double vol)
    :
    m_vol(vol)
{
    PRE_CONDITION(vol >= 0.0);
}

double COptionNormal::CallPremium(
    double fwd, double expiry, double strike) const
{
    PRE_CONDITION(fwd > 0.0);
    PRE_CONDITION(expiry > 0.0);
    PRE_CONDITION(strike > 0.0);

    double volSqrtExpiry = m_vol * sqrt(expiry);
    double d1 = (fwd - strike) / volSqrtExpiry;
    double premium = volSqrtExpiry * NormDist::PHI(d1) + 
	(fwd-strike) * NormDist::CND(d1);

    return premium;
}

double COptionNormal::CallDelta(
    double fwd, double expiry, double strike) const
{
    PRE_CONDITION(fwd > 0.0);
    PRE_CONDITION(expiry > 0.0);
    PRE_CONDITION(strike > 0.0);

    double volSqrtExpiry = m_vol * sqrt(expiry);
    double d1 = (fwd - strike) / volSqrtExpiry;
    double delta = NormDist::PHI(d1);

    return delta;
}

double COptionNormal::Volatility() const
{
    return m_vol;
}

void COptionNormal::SetVolatility(double volatility)
{
    PRE_CONDITION(volatility >= 0.0);
    m_vol = volatility;
}

COptionDist* COptionNormal::Clone() const
{
    return new COptionNormal(m_vol);
}

