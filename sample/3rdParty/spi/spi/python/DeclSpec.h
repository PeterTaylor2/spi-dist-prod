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
** SPI_PY_IMPORT is set for Microsoft compilations. If SPI_PY_EXPORT is defined
** then it defines SPI_PY_IMPORT as dllexport otherwise as dllimport.
**
** Thus when building define -DSPI_PY_EXPORT. When consuming no definitions
** are needed.
***************************************************************************
*/

#ifndef SPI_PY_DECLSPEC_H
#define SPI_PY_DECLSPEC_H

#ifdef _MSC_VER

/* microsoft compiler */

#ifdef SPI_PY_EXPORT
#define SPI_PY_IMPORT __declspec(dllexport)
#else
#define SPI_PY_IMPORT __declspec(dllimport)
#endif

#else

/* not using a microsoft compiler */
/* we should also implement something similar for modern gcc compilers */
#define SPI_PY_IMPORT

#endif

#endif



