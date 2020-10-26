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
**
** At present this is empty, but we will doubtless find things to add as
** we go along!
***************************************************************************
*/

#ifndef SPI_PLATFORM_H
#define SPI_PLATFORM_H

#include <spi_util/platform.h>

#ifdef __GNUC__

/* gcc */

#define GCC_VERSION (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__)

#else

#define GCC_VERSION 0

#endif

#ifdef _MSC_VER

/* microsoft */

// removes compiler warning about non-dll exported classes inside
// other classes where some of the functions might be inlined
// tried to fix the problem where it came up, but failed - so for
// the moment we will suppress the warning
#pragma warning(disable : 4251)

#define strdup _strdup
#define strcasecmp _strcmpi

#if _MSC_VER <= 1600

/* Visual Studio 2010 or earlier */

#define isnan _isnan
#define isinf !_finite

#endif

#endif

#endif
