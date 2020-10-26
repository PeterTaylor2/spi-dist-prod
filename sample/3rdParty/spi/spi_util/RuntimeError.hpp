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
** RuntimeError.hpp
***************************************************************************
** Defines the RuntimeError functions which can be used for convenience
** to throw std::runtime_error.
**
** We can use either printf-style formatting via the RuntimeError functions.
** Or else c++ streams via the SPI_UTIL_THROW_RUNTIME_ERROR macro.
***************************************************************************
*/

#ifndef SPI_UTIL_RUNTIME_ERROR_HPP
#define SPI_UTIL_RUNTIME_ERROR_HPP

#include "DeclSpec.h"
#include "Namespace.hpp"
#include <stdexcept>
#include <sstream>
#include <stdarg.h>

#undef SPI_UTIL_PRE_CONDITION
#define SPI_UTIL_PRE_CONDITION(cond) do { if(!(cond)) {\
throw spi_util::RuntimeError("%s: Required pre-condition (%s) fails", __FUNCTION__, #cond);\
}} while (0)

#undef SPI_UTIL_POST_CONDITION
#define SPI_UTIL_POST_CONDITION(cond) do { if(!(cond)) {\
throw spi_util::RuntimeError("%s: Required post-condition (%s) fails", __FUNCTION__, #cond);\
}} while (0)

SPI_UTIL_NAMESPACE

SPI_UTIL_IMPORT
std::runtime_error RuntimeErrorV(const char* format, va_list args);

SPI_UTIL_IMPORT
std::runtime_error RuntimeError(const char* format, ...);

SPI_UTIL_IMPORT
std::runtime_error RuntimeError(std::exception&e, const char* routine);

SPI_UTIL_END_NAMESPACE

#define SPI_UTIL_THROW_RUNTIME_ERROR(msg) { \
std::ostringstream oss_;\
oss_ << msg;\
throw std::runtime_error(oss_.str());\
}

#define SPI_UTIL_NOT_IMPLEMENTED throw spi_util::RuntimeError("%s: Not implemented", __FUNCTION__)

#endif /* SPI_UTIL_RUNTIME_ERROR_HPP */

