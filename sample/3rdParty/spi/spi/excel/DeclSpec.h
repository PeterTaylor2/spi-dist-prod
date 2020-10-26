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
** SPI_XL_IMPORT is set for Microsoft compilations. If SPI_XL_EXPORT is defined
** then it defines SPI_XL_IMPORT as dllexport otherwise as dllimport.
**
** Thus when building define -DSPI_XL_EXPORT. When consuming no definitions
** are needed.
***************************************************************************
*/

#ifndef SPI_XL_DECLSPEC_H
#define SPI_XL_DECLSPEC_H

/* this code is only compiled by microsoft compilers */

#ifdef SPI_XL_EXPORT
#define SPI_XL_IMPORT __declspec(dllexport)
#else
#define SPI_XL_IMPORT __declspec(dllimport)
#endif

#endif



