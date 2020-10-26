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

#include "zeroCurve.h"

#include <isda/cerror.h>
#include <isda/macros.h>
#include <isda/tcurve.h>

TZeroCurve* ZeroCurveMake(
    TCurve* tc,
    long interpType)
{
    static const char* routine = __FUNCTION__;
    int status = FAILURE;
    TZeroCurve* out = NULL;

    REQUIRE(tc != NULL);

    out = NEW(TZeroCurve);
    if (!out)
        goto done; /* failure */

    out->tc         = tc;
    out->interpType = interpType;

    status = SUCCESS;

done:

    if (status != SUCCESS)
    {
        JpmcdsErrMsgFailure((char*)routine);
        JpmcdsFreeTCurve(tc);
        return NULL;
    }

    return out;
}

TZeroCurve* ZeroCurveCopy(
    TZeroCurve* zeroCurve)
{
    if (!zeroCurve)
        return NULL;

    return ZeroCurveMake(
        JpmcdsCopyCurve(zeroCurve->tc),
        zeroCurve->interpType);
}

void ZeroCurveFree(
    TZeroCurve* zeroCurve)
{
    if (zeroCurve)
    {
        JpmcdsFreeTCurve(zeroCurve->tc);
        FREE(zeroCurve);
    }
}


