/*

Copyright (C) 2012-2019 Sartorial Programming Ltd.

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
/*
***************************************************************************
** platform.h
***************************************************************************
** Some platform-specific definitions. This file should only depend upon
** macros defined by the compiler vendors.
***************************************************************************
*/

#ifndef SPI_UTIL_PLATFORM_H
#define SPI_UTIL_PLATFORM_H

#ifdef __GNUC__

/* gcc */

#include <inttypes.h>

#endif

#ifdef _MSC_VER

/* microsoft */

#if _MSC_VER <= 1600

/* Visual Studio 2010 or earlier */

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;

#else

#include <inttypes.h>

#endif


#endif

#endif
