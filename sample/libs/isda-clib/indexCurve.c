/*

Copyright (C) 2015 Sartorial Programming Ltd.

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

#include "indexCurve.h"
#include "utils.h"

#include <isda/bsearch.h>
#include <isda/cerror.h>
#include <isda/convert.h>
#include <isda/ldate.h>
#include <isda/macros.h>
#include <isda/strutil.h>
#include <isda/zr2fwd.h>

/*
**************************************************************************
** Implementation of TIndexCurveType
**************************************************************************
*/
TIndexCurveType* IndexCurveTypeMake(
    char* curveName,
    TDateInterval* rateTenor,
    long rateDayCountConv,
    TDateInterval* fixedSwapInterval,
    long fixedSwapDayCountConv,
    TDateInterval* floatSwapInterval,
    long floatSwapDayCountConv,
    long badDayConv,
    char* holidayFile)
{
    static const char* routine = __FUNCTION__;
    int status = FAILURE;
    TIndexCurveType* out = NULL;

    REQUIRE(curveName != NULL);
    REQUIRE(rateTenor != NULL);
    REQUIRE(fixedSwapInterval != NULL);
    REQUIRE(floatSwapInterval != NULL);
    REQUIRE(holidayFile != NULL);

    out = NEW(TIndexCurveType);
    if (!out)
        goto done; /* failure */

    out->curveName = JpmcdsStringDuplicate(curveName);
    if (!out->curveName)
        goto done; /* failure */

    out->rateTenor             = *rateTenor;
    out->rateDayCountConv      = rateDayCountConv;
    out->fixedSwapInterval     = *fixedSwapInterval;
    out->fixedSwapDayCountConv = fixedSwapDayCountConv;
    out->floatSwapInterval     = *floatSwapInterval;
    out->floatSwapDayCountConv = floatSwapDayCountConv;
    out->badDayConv            = badDayConv;
    out->holidayFile           = JpmcdsStringDuplicate(holidayFile);
    if (!out->holidayFile)
        goto done; /* failure */

    status = SUCCESS;

done:

    if (status != SUCCESS)
    {
        JpmcdsErrMsgFailure((char*)__FUNCTION__);
        IndexCurveTypeFree(out);
        return NULL;
    }

    return out;
}

TIndexCurveType* IndexCurveTypeCopy(
    TIndexCurveType* indexCurveType)
{
    if (!indexCurveType)
        return NULL;

    return IndexCurveTypeMake(
        indexCurveType->curveName,
        &indexCurveType->rateTenor,
        indexCurveType->rateDayCountConv,
        &indexCurveType->fixedSwapInterval,
        indexCurveType->fixedSwapDayCountConv,
        &indexCurveType->floatSwapInterval,
        indexCurveType->floatSwapDayCountConv,
        indexCurveType->badDayConv,
        indexCurveType->holidayFile);
}

void IndexCurveTypeFree(
    TIndexCurveType* indexCurveType)
{
    if (indexCurveType)
    {
        FREE(indexCurveType->curveName);
        FREE(indexCurveType);
    }
}

/*
**************************************************************************
** Implementation of TIndexCurveFixings
**************************************************************************
*/
TIndexCurveFixings* IndexCurveFixingsMake(
    int numFixings,
    TRatePt* fixings)
{
    static const char* routine = __FUNCTION__;
    int status = FAILURE;
    TIndexCurveFixings* out = NULL;
    int i;

    REQUIRE(numFixings >= 0);
    if (numFixings > 0)
    {
        REQUIRE(fixings != NULL);
    }

    for (i = 1; i < numFixings; ++i)
    {
        if (fixings[i].fDate <= fixings[i-1].fDate)
        {
            JpmcdsErrMsg("%s: Dates not in ascending order", __FUNCTION__);
            goto done; /* failure */
        }
    }

    out = NEW(TIndexCurveFixings);
    if (!out)
        goto done; /* failure */

    out->numFixings = numFixings;
    if (numFixings > 0)
    {
        out->fixings = NEW_ARRAY(TRatePt, numFixings);
        if (!out->fixings)
            goto done; /* failure */
        COPY_ARRAY(out->fixings, fixings, TRatePt, numFixings);
    }

    status = SUCCESS;

done:

    if (status != SUCCESS)
    {
        IndexCurveFixingsFree(out);
        JpmcdsErrMsgFailure((char*)__FUNCTION__);
        return NULL;
    }

    return out;
}

TIndexCurveFixings* IndexCurveFixingsCopy(
    TIndexCurveFixings* indexCurveFixings)
{
    if (!indexCurveFixings)
        return NULL;

    return IndexCurveFixingsMake(
        indexCurveFixings->numFixings,
        indexCurveFixings->fixings);
}

void IndexCurveFixingsFree(
    TIndexCurveFixings* indexCurveFixings)
{
    if (indexCurveFixings)
    {
        FREE(indexCurveFixings->fixings);
        FREE(indexCurveFixings);
    }
}

/*
 * Finds the fixing for a given effective date.
 *
 * It is acceptable not to have a fixing for that date when:
 *  1. fixings is NULL.
 *  2. fixings->numFixings == 0
 *  3. effectiveDate is after the last date in fixings->fixings
 *
 * However if effectiveDate is on or before the last date in fixings->fixings
 * then we insist that the effectiveDate is found or else we return FAILURE.
 *
 * Note that TIndexCurveFixings dates are validated to be in ascending
 * order on construction of the type.
 */
int IndexCurveFixingsFind(
    TIndexCurveFixings* fixings,
    TDate effectiveDate,
    TBoolean* foundIt,
    double* fixing)
{
    static const char* routine = __FUNCTION__;
    int status = FAILURE;

    REQUIRE(foundIt != NULL);
    REQUIRE(fixing != NULL);

    if (!fixings || fixings->numFixings == 0)
    {
        status = SUCCESS;
        *foundIt = FALSE;
        *fixing = MakeNan();
        goto done; /* success */
    }

    REQUIRE(fixings->numFixings > 0);
    REQUIRE(fixings->fixings != NULL);

    if (effectiveDate > fixings->fixings[fixings->numFixings-1].fDate)
    {
        *foundIt = FALSE;
        *fixing = MakeNan();
    }
    else
    {
        int lo;
        int hi;
        if (JpmcdsBSearchLongFast(
            effectiveDate,
            &fixings->fixings[0].fDate,
            sizeof(TRatePt),
            fixings->numFixings,
            &lo,
            &hi) != SUCCESS)
        {
            goto done; /* failure */
        }

        if (lo >= 0 && 
            lo < fixings->numFixings && 
            fixings->fixings[lo].fDate == effectiveDate)
        {
            *foundIt = TRUE;
            *fixing = fixings->fixings[lo].fRate;
        }
        else if (hi >= 0 && 
            hi < fixings->numFixings && 
            fixings->fixings[hi].fDate == effectiveDate)
        {
            *foundIt = TRUE;
            *fixing = fixings->fixings[hi].fRate;
        }
        else
        {
            JpmcdsErrMsg("%s: Could not find fixing for date %s",
                routine, JpmcdsFormatDate(effectiveDate));
            goto done; /* failure */
        }
    }

    status = SUCCESS;

done:

    if (status != SUCCESS)
    {
        JpmcdsErrMsgFailure((char*)routine);
        return FAILURE;
    }

    return SUCCESS;
}



/*
**************************************************************************
** Implementation of TIndexCurve
**************************************************************************
*/
TIndexCurve* IndexCurveMake(
    TZeroCurve* zc,
    TIndexCurveType* curveType,
    TIndexCurveFixings* curveFixings)
{
    static const char* routine = __FUNCTION__;
    int status = FAILURE;

    TIndexCurve* out = NULL;

    REQUIRE(zc != NULL);
    REQUIRE(curveType != NULL);

    out = NEW(TIndexCurve);
    if (!out)
        goto done; /* failure */

    out->zc = ZeroCurveCopy(zc);
    if (!out->zc)
        goto done; /* failure */

    out->curveType = IndexCurveTypeCopy(curveType);
    if (!out->curveType)
        goto done; /* failure */

    if (curveFixings)
    {
        out->curveFixings = IndexCurveFixingsCopy(curveFixings);
        if (!out->curveFixings)
            goto done; /* failure */
    }

    status = SUCCESS;

done:

    if (status != SUCCESS)
    {
        IndexCurveFree(out);
        JpmcdsErrMsgFailure((char*)routine);
        return NULL;
    }

    return out;
}

TIndexCurve* IndexCurveCopy(
    TIndexCurve* indexCurve)
{
    if (!indexCurve)
        return NULL;

    return IndexCurveMake(
        indexCurve->zc,
        indexCurve->curveType,
        indexCurve->curveFixings);
}

void IndexCurveFree(
    TIndexCurve* indexCurve)
{
    if (indexCurve)
    {
        IndexCurveFixingsFree(indexCurve->curveFixings);
        IndexCurveTypeFree(indexCurve->curveType);
        ZeroCurveFree(indexCurve->zc);
        FREE(indexCurve);
    }
}

/*
 * Finds the index rate for a curve and effective date.
 *
 * This can either be a fixed rate if the effective date is on or before
 * the last available fixing date, or an estimated rate from the curve.
 *
 * This is indicated by the rateIsFixed parameter.
 */
int IndexCurveRate(
    TIndexCurve* indexCurve,
    TDate effectiveDate,
    TBoolean* rateIsFixed,
    double* rate)
{
    static const char* routine = __FUNCTION__;
    int status = FAILURE;

    REQUIRE(indexCurve != NULL);
    REQUIRE(rateIsFixed != NULL);
    REQUIRE(rate != NULL);

    if (IndexCurveFixingsFind(
        indexCurve->curveFixings, 
        effectiveDate,
        rateIsFixed,
        rate) != SUCCESS)
    {
        goto done; /* failure */
    }

    if (!*rateIsFixed)
    {
        TDate maturityDate;
        if (JpmcdsDateFwdThenAdjust(
            effectiveDate,
            &indexCurve->curveType->rateTenor,
            indexCurve->curveType->badDayConv,
            indexCurve->curveType->holidayFile,
            &maturityDate) != SUCCESS)
        {
            goto done; /* failure */
        }

        if (effectiveDate < indexCurve->zc->tc->fBaseDate)
        {
            JpmcdsErrMsg("%s: Effective date %s is before base date of %s curve %s",
                routine,
                JpmcdsFormatDate(effectiveDate),
                indexCurve->curveType->curveName,
                JpmcdsFormatDate(indexCurve->zc->tc->fBaseDate));
            goto done; /* failure */
        }

        if (JpmcdsForwardFromZCurve(
            indexCurve->zc->tc,
            indexCurve->zc->interpType,
            effectiveDate,
            maturityDate,
            indexCurve->curveType->rateDayCountConv,
            JPMCDS_SIMPLE_BASIS,
            rate) != SUCCESS)
        {
            goto done; /* failure */
        }
    }

    status = SUCCESS;

done:

    if (status != SUCCESS)
    {
        JpmcdsErrMsgFailure((char*)routine);
        return FAILURE;
    }

    return SUCCESS;
}



