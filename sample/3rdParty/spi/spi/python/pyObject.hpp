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
#ifndef SPI_PY_OBJECT_HPP
#define SPI_PY_OBJECT_HPP

/*
***************************************************************************
** pyObject.hpp
**
** Python equivalent to spi::Object
***************************************************************************
*/

#include "pyObject.h"

#include "pyUtil.hpp"
#include "pyValue.hpp"
#include "pyOutput.hpp"

#include "../Value.hpp"
#include "../Object.hpp"

SPI_BEGIN_NAMESPACE

SPI_PY_IMPORT
const Object* pyInitConstObject(
    PyObject* args, FunctionCaller* func, ObjectType* type);

ObjectConstSP spiPyObjectGetObject(SpiPyObject* pyo);

SPI_END_NAMESPACE

#endif
