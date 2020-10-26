#ifndef ISDA_CLIB_ZERO_CURVE_H
#define ISDA_CLIB_ZERO_CURVE_H

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

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _TZeroCurve
{
    TCurve* tc;
    long interpType;
} TZeroCurve;

/*
 * Creates TZeroCurve taking ownership of TCurve.
 */
TZeroCurve* ZeroCurveMake(
    TCurve* tc,
    long interpType);

TZeroCurve* ZeroCurveCopy(
    TZeroCurve* zeroCurve);

void ZeroCurveFree(
    TZeroCurve* zeroCurve);

#ifdef __cplusplus
}
#endif

#endif
