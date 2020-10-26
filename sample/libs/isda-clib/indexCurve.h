#ifndef ISDA_CLIB_INDEX_CURVE_H
#define ISDA_CLIB_INDEX_CURVE_H

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

#include <isda/bastypes.h>
#include "zeroCurve.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Defines curve type - e.g. holidays, day count conventions, intervals etc.
 */
typedef struct _TIndexCurveType
{
    char* curveName;
    TDateInterval rateTenor;
    long rateDayCountConv;
    TDateInterval fixedSwapInterval;
    long fixedSwapDayCountConv;
    TDateInterval floatSwapInterval;
    long floatSwapDayCountConv;
    long badDayConv;
    char* holidayFile;
} TIndexCurveType;

TIndexCurveType* IndexCurveTypeMake(
    char* curveName,
    TDateInterval* rateTenor,
    long rateDayCountConv,
    TDateInterval* fixedSwapInterval,
    long fixedSwapDayCountConv,
    TDateInterval* floatSwapInterval,
    long floatSwapDayCountConv,
    long badDayConv,
    char* holidayFile);

TIndexCurveType* IndexCurveTypeCopy(
    TIndexCurveType* indexCurveType);

void IndexCurveTypeFree(
    TIndexCurveType* indexCurveType);

/*
 * Defines fixings for a particular index curve.
 * The dates are the effective dates for the rates to be fixed.
 */
typedef struct _TIndexCurveFixings
{
    int numFixings;
    TRatePt* fixings;
} TIndexCurveFixings;

TIndexCurveFixings* IndexCurveFixingsMake(
    int numFixings,
    TRatePt* fixings);

TIndexCurveFixings* IndexCurveFixingsCopy(
    TIndexCurveFixings* indexCurveFixings);

void IndexCurveFixingsFree(
    TIndexCurveFixings* indexCurveFixings);

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
    double* fixing);

/*
 * Defines an index curve together with recent fixings of the rate.
 */
typedef struct _TIndexCurve
{
    TZeroCurve* zc;
    TIndexCurveType* curveType;
    TIndexCurveFixings* curveFixings;
} TIndexCurve;


TIndexCurve* IndexCurveMake(
    TZeroCurve* zc,
    TIndexCurveType* curveType,
    TIndexCurveFixings* curveFixings);

TIndexCurve* IndexCurveCopy(
    TIndexCurve* indexCurve);

void IndexCurveFree(
    TIndexCurve* indexCurve);

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
    double* rate);

#ifdef __cplusplus
}
#endif

#endif
