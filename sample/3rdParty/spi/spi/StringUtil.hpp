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
/**
***************************************************************************
** StringUtil.hpp
***************************************************************************
** String utility functions.
***************************************************************************
*/

#ifndef SPI_STRING_UTIL_HPP
#define SPI_STRING_UTIL_HPP

#include "Namespace.hpp"

#include "../spi_util/StringUtil.hpp"

SPI_BEGIN_NAMESPACE

/*
 * We don't redeclare or redirect all the useful functions from spi_util.
 *
 * Instead we rename them so that you can call spi::StringFormat (for
 * example) and you will actually be calling spi_util::StringFormat (in
 * that case).
 */

using spi_util::StringFormatV;
using spi_util::StringFormat;
using spi_util::StringMapFormat;
using spi_util::StringParser;
using spi_util::StringSplit;
using spi_util::StringJoin;
using spi_util::StringReplace;
using spi_util::StringStrip;
using spi_util::StringUpper;
using spi_util::StringLower;
using spi_util::StringCap;
using spi_util::StringToLong;
using spi_util::StringToInt;
using spi_util::StringToDouble;
using spi_util::StringEscape;
using spi_util::StringCapture;
using spi_util::StringStartsWith;
using spi_util::StringEndsWith;

using spi_util::CStringPreProcess;

SPI_END_NAMESPACE

#endif


