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
#ifndef SPI_XL_INPUT_HPP
#define SPI_XL_INPUT_HPP

/**
 * Functions to convert from Excel inputs (XLOPER) to the fundamental types
 * of the SPI library.
 */

#include "DeclSpec.h"

#include <string>

#include <spi/Namespace.hpp>

typedef struct xloper XLOPER;

#if SPI_XL_VERSION == 12
typedef struct xloper12 XLOPER12;
#endif

SPI_BEGIN_NAMESPACE

class Value;

class SPI_XL_IMPORT ExcelInputError
{
public:
    ExcelInputError();
    const char* what() const;
};

bool xloperToBool(
    XLOPER* oper,
    const char* name,
    bool optional=false,
    bool defaultValue=false);

std::string xloperToString(
    XLOPER* oper,
    const char* name,
    bool optional=false,
    const char* defaultValue="");

#if SPI_XL_VERSION == 12

std::string xloper12ToString(
    XLOPER12* oper,
    const char* name,
    bool optional = false,
    const char* defaultValue = "");

#endif

SPI_END_NAMESPACE

#endif // SPI_XL_INPUT_HPP

