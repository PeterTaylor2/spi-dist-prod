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
** Or else c++ streams via the SPI_THROW_RUNTIME_ERROR macro.
***************************************************************************
*/

#ifndef SPI_RUNTIME_ERROR_HPP
#define SPI_RUNTIME_ERROR_HPP

#include "DeclSpec.h"
#include "Namespace.hpp"
#include <stdexcept>
#include <sstream>
#include <stdarg.h>

#undef SPI_PRE_CONDITION
#define SPI_PRE_CONDITION(cond) do { if(!(cond)) {\
throw spi::RuntimeError("%s: Required pre-condition (%s) fails", __FUNCTION__, #cond);\
}} while (0)

#undef SPI_POST_CONDITION
#define SPI_POST_CONDITION(cond) do { if(!(cond)) {\
throw spi::RuntimeError("%s: Required post-condition (%s) fails", __FUNCTION__, #cond);\
}} while (0)

SPI_BEGIN_NAMESPACE

SPI_IMPORT
std::runtime_error RuntimeError(const char* format, ...);

SPI_IMPORT
std::runtime_error RuntimeError(std::exception&e, const char* routine);

SPI_END_NAMESPACE

#define SPI_THROW_RUNTIME_ERROR(msg) { \
std::ostringstream oss_;\
oss_ << msg;\
throw std::runtime_error(oss_.str());\
}

#define SPI_NOT_IMPLEMENTED throw spi::RuntimeError("%s: Not implemented", __FUNCTION__)

#endif /* SPI_RUNTIME_ERROR_HPP */

