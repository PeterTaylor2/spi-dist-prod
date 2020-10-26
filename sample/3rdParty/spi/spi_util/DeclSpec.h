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
** DeclSpec.h
**
** Defines various declaration specifications. Windows specific.
**
** SPI_UTIL_IMPORT is set for Microsoft compilations. If SPI_UTIL_EXPORT is defined
** then it defines SPI_UTIL_IMPORT as dllexport otherwise as dllimport.
**
** Thus when building define -DSPI_UTIL_EXPORT. When consuming no definitions
** are needed.
***************************************************************************
*/

#ifndef SPI_UTIL_DECLSPEC_H
#define SPI_UTIL_DECLSPEC_H

#ifdef SPI_UTIL_STATIC

#define SPI_UTIL_IMPORT

#else

#ifdef _MSC_VER

/* microsoft compiler */

#ifdef SPI_UTIL_EXPORT
#define SPI_UTIL_IMPORT __declspec(dllexport)
#else
#define SPI_UTIL_IMPORT __declspec(dllimport)
#endif

// this is needed because MSVC emits warnings about use of STL-types from DLLs
// we believe it to be meaningless as long as you use the same STL throughout
#pragma warning(disable:4251)

#else

/* not using a microsoft compiler */
/* we should also implement something similar for modern gcc compilers */
#define SPI_UTIL_IMPORT

#endif

#endif

#endif


