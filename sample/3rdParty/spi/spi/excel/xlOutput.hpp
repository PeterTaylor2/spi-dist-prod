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
#ifndef SPI_XL_OUTPUT_HPP
#define SPI_XL_OUTPUT_HPP

/**
 * Functions to convert from Excel outputs (XLOPER) to the fundamental types
 * of the SPI library.
 */

#include "DeclSpec.h"

#include <spi/Date.hpp>
#include <spi/Object.hpp>

#include <string>
#include <vector>

typedef struct xloper XLOPER;

SPI_BEGIN_NAMESPACE

SPI_XL_IMPORT XLOPER* xloperOutput(XLOPER* xlo);
SPI_XL_IMPORT XLOPER* xloperInFunctionWizard();
SPI_XL_IMPORT XLOPER* xloperInputError();
XLOPER* xloperError(int errorCode);
SPI_XL_IMPORT XLOPER* xloperFromBool(bool value);

SPI_END_NAMESPACE

#endif // SPI_XL_OUTPUT_HPP

