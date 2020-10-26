/*
 * ISDA CDS Standard Model
 *
 * Copyright (C) 2009 International Swaps and Derivatives Association, Inc.
 * Developed and supported in collaboration with Markit
 * 
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the ISDA CDS Standard Model Public License.
 */

#include "contingentleg.h"
#include <math.h>
#include "timeline.h"
#include "macros.h"
#include "cxdatelist.h"
#include "cxzerocurve.h"
#include "busday.h"
#include "dtlist.h"
#include "cashflow.h"
#include "cerror.h"


/*
***************************************************************************
** Computes a one period integral
***************************************************************************
*/
static int onePeriodIntegral
(TDate             today,
 TDate             startDate,
 TDate             endDate,
 TCurve           *discountCurve,
 TCurve           *spreadCurve,
 double            recoveryRate,
 double           *pv);


/*
***************************************************************************
** Computes a one period integral with payment at a specific payment date.
***************************************************************************
*/
static int onePeriodIntegralAtPayDate
(TDate             today,
 TDate             startDate,
 TDate             endDate,
 TDate             payDate, 
 TCurve           *discountCurve,
 TCurve           *spreadCurve,
 double            recoveryRate,
 double           *pv);


/*
***************************************************************************
** Computes the PV of a contingent leg as a whole.
**
** For each payment period this is the integral of LGD(t) . Z(t) . dS/dt dt
** where S is the survival function and LGD is the loss given default
** function and Z is the discount function. Discounting is calculated at the
** payment date and not at the observation date.
***************************************************************************
*/
int JpmcdsContingentLegPV
(TContingentLeg *cl,               /* (I) Contingent leg                  */
 TDate           today,            /* (I) No observations before today    */
 TDate           valueDate,        /* (I) Value date for discounting      */
 TDate           stepinDate,
 TCurve         *discountCurve,    /* (I) Risk-free curve                 */
 TCurve         *spreadCurve,      /* (I) Spread curve                    */
 double          recoveryRate,     /* (I) Recovery rate                   */
 double         *pv)               /* (O) Present value of contingent leg */
{
    static char routine[] = "JpmcdsContingentLegPV";
    int         status    = FAILURE;

    double myPv = 0.0;
    double valueDatePv;
    TDate startDate;

    int   offset;

    REQUIRE (cl != NULL);
    REQUIRE (discountCurve != NULL);
    REQUIRE (spreadCurve != NULL);
    REQUIRE (pv != NULL);

    offset = (cl->protectStart ? 1 : 0);
    startDate = MAX(cl->startDate, stepinDate - offset);
    startDate = MAX(startDate, today - offset);

    switch (cl->payType)
    {
    case PROT_PAY_MAT:
        {
            double tmp;
            if (onePeriodIntegralAtPayDate (today,
                                            startDate,
                                            cl->endDate,
                                            cl->endDate,
                                            discountCurve,
                                            spreadCurve,
                                            recoveryRate,
                                            &tmp) != SUCCESS)
                goto done;
            
            myPv += tmp * cl->notional;
        }
        break;
    case PROT_PAY_DEF:
        {
            double tmp;
            if (onePeriodIntegral (today,
                                   startDate,
                                   cl->endDate,
                                   discountCurve,
                                   spreadCurve,
                                   recoveryRate,
                                   &tmp) != SUCCESS)
            goto done;
            
            myPv += tmp * cl->notional;
        }
        break;
    default:
        JpmcdsErrMsg ("%s: Invalid payment type %d\n", routine, (int)cl->payType);
        goto done;
    }

    /* myPv has been calculated as at today - need it at valueDate */
    valueDatePv = JpmcdsForwardZeroPrice (discountCurve, today, valueDate);

    status = SUCCESS;
    *pv    = myPv / valueDatePv;

 done:

    if (status != SUCCESS)
        JpmcdsErrMsgFailure (routine);

    return status;
}


/*
***************************************************************************
** Computes a one period integral
***************************************************************************
*/
static int onePeriodIntegral
(TDate             today,
 TDate             startDate,
 TDate             endDate,
 TCurve           *discCurve,
 TCurve           *spreadCurve,
 double            recoveryRate,
 double           *pv)
{
    static char routine[] = "onePeriodIntegral";
    int         status    = FAILURE;

    double  myPv = 0.0;
    int     i;

    double t;
    double s0;
    double s1;
    double df0;
    double df1;
    double loss;

    TDateList *tl = NULL;

    REQUIRE (endDate > startDate);
    REQUIRE (discCurve != NULL);
    REQUIRE (spreadCurve != NULL);
    REQUIRE (pv != NULL);

    if (today > endDate)
    {
        myPv = 0.0;
        goto success;
    }

    tl = JpmcdsRiskyTimeLine (startDate, endDate, discCurve, spreadCurve);
    if (tl == NULL)
        goto done;

    /* the integration - we can assume flat forwards between points on
       the timeline - this is true for both curves 

       we are integrating -Z dS/dt where Z is the discount factor and
       S is the survival probability

       assuming flat forwards on each part of the integration, this is an
       exact integral
    */

    s1  = JpmcdsForwardZeroPrice(spreadCurve, today, startDate);
    df1 = JpmcdsForwardZeroPrice(discCurve, today, MAX(today, startDate));
    loss = 1.0 - recoveryRate;

    for (i = 1; i < tl->fNumItems; ++i)
    {
        double lambda;
        double fwdRate;
        double thisPv;

        s0  = s1;
        df0 = df1;
        s1  = JpmcdsForwardZeroPrice(spreadCurve, today, tl->fArray[i]);
        df1 = JpmcdsForwardZeroPrice(discCurve, today, tl->fArray[i]);
        t   = (double)(tl->fArray[i] - tl->fArray[i-1])/365.0;
        
        lambda  = log(s0/s1)/t;
        fwdRate = log(df0/df1)/t;
        
        thisPv  = loss * lambda / (lambda + fwdRate) * 
            (1.0 - exp(-(lambda + fwdRate) * t)) * s0 * df0;
        
        myPv += thisPv;
    }

 success:

    status = SUCCESS;
    *pv = myPv;
        
 done:

    if (status != SUCCESS)
        JpmcdsErrMsgFailure (routine);

    JpmcdsFreeDateList (tl);
    return status;
}


/*
***************************************************************************
** Computes a one period integral with payment at a specific payment date.
** This is actually trivial.
***************************************************************************
*/
static int onePeriodIntegralAtPayDate
(TDate             today,
 TDate             startDate,
 TDate             endDate,
 TDate             payDate, 
 TCurve           *discCurve,
 TCurve           *spreadCurve,
 double            recoveryRate,
 double           *pv)
{
    static char routine[] = "onePeriodIntegralAtPayDate";
    int         status    = FAILURE;

    double df;
    double s0;
    double s1;
    double loss;

    REQUIRE (endDate > startDate);
    REQUIRE (discCurve != NULL);
    REQUIRE (spreadCurve != NULL);
    REQUIRE (pv != NULL);

    if (today > endDate)
    {
        *pv = 0.0;
    }
    else
    {
        s0  = JpmcdsForwardZeroPrice(spreadCurve, today, startDate);
        s1  = JpmcdsForwardZeroPrice(spreadCurve, today, endDate);
        df  = JpmcdsForwardZeroPrice(discCurve, today, payDate);
        loss = 1.0 - recoveryRate;
        *pv = (s0 - s1) * df * loss;
    }

    status = SUCCESS;
        
 done:

    if (status != SUCCESS)
        JpmcdsErrMsgFailure (routine);
    
    return status;
}
