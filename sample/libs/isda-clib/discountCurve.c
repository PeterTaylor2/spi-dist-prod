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

#include "discountCurve.h"

#include <isda/cerror.h>
#include <isda/macros.h>
#include <isda/tcurve.h>

TDiscountCurve* DiscountCurveMake(
    TDate anchorDate,
    TZeroCurve* zc)
{
    static const char* routine = __FUNCTION__;
    int status = FAILURE;
    TDiscountCurve* out = NEW(TDiscountCurve);

    out->anchorDate = anchorDate;
    out->zc         = ZeroCurveCopy(zc);

    if (!out->zc)
        goto done; /* failure */

    status = SUCCESS;

done:

    if (status != SUCCESS)
    {
        JpmcdsErrMsgFailure((char*)routine);
        DiscountCurveFree(out);
        return NULL;
    }

    return out;
}

TDiscountCurve* DiscountCurveCopy(
    TDiscountCurve* discountCurve)
{
    if (!discountCurve)
        return NULL;

    return DiscountCurveMake(
        discountCurve->anchorDate,
        discountCurve->zc);
}

void DiscountCurveFree(
    TDiscountCurve* discountCurve)
{
    if (discountCurve)
    {
        ZeroCurveFree(discountCurve->zc);
        FREE(discountCurve);
    }
}

int DiscountCurveDiscount(
    TDiscountCurve* discountCurve,
    TDate date,
    double* discount)
{
    static const char* routine = __FUNCTION__;
    int status = FAILURE;
    double anchorDiscount;
    double dateDiscount;

    REQUIRE(discountCurve != NULL);
    REQUIRE(discount != NULL);

    if (JpmcdsDiscountDate(
        discountCurve->anchorDate,
        discountCurve->zc->tc,
        discountCurve->zc->interpType,
        &anchorDiscount) != SUCCESS)
    {
        goto done; /* failure */
    }

    if (JpmcdsDiscountDate(
        date,
        discountCurve->zc->tc,
        discountCurve->zc->interpType,
        &dateDiscount) != SUCCESS)
    {
        goto done; /* failure */
    }

    *discount = dateDiscount / anchorDiscount;
    status = SUCCESS;

done:

    if (status != SUCCESS)
    {
        JpmcdsErrMsgFailure((char*)routine);
        return FAILURE;
    }

    return SUCCESS;
}

int DiscountCurveFwdDiscount(
    TDiscountCurve* discountCurve,
    TDate startDate,
    TDate endDate,
    double* discount)
{
    static const char* routine = __FUNCTION__;
    int status = FAILURE;
    double startDiscount;
    double endDiscount;

    REQUIRE(discountCurve != NULL);
    REQUIRE(discount != NULL);

    if (JpmcdsDiscountDate(
        startDate,
        discountCurve->zc->tc,
        discountCurve->zc->interpType,
        &startDiscount) != SUCCESS)
    {
        goto done; /* failure */
    }

    if (JpmcdsDiscountDate(
        endDate,
        discountCurve->zc->tc,
        discountCurve->zc->interpType,
        &endDiscount) != SUCCESS)
    {
        goto done; /* failure */
    }

    *discount = endDiscount / startDiscount;
    status = SUCCESS;

done:

    if (status != SUCCESS)
    {
        JpmcdsErrMsgFailure((char*)routine);
        return FAILURE;
    }

    return SUCCESS;
}

