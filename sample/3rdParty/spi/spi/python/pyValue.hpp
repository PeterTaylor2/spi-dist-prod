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
** pyValue.hpp
**
** Mapping between spi::Value and PyObject
**
** Note that all functions are in the spi namespace, but also use the
** prefix py (lower case). This is to distinguish them from the Python
** C-interface which uses the prefix Py (mixed case). Otherwise there
** might be name clashes which would require us to use :: before functions
** from the Python C-interface.
***************************************************************************
*/

#ifndef SPI_PYVALUE_HPP
#define SPI_PYVALUE_HPP

#include "Python.h"

#include "pyUtil.hpp"
#include <spi/Value.hpp>
#include <spi/InputValues.hpp>

#include <exception>
#include <string>
#include <vector>

SPI_BEGIN_NAMESPACE

SPI_PY_IMPORT
PyObject* pyoFromValue(const Value& value);

SPI_PY_IMPORT
PyObject* pyoFromMultiValue(const Value& value, size_t numVars);

SPI_PY_IMPORT
Value pyoToValue(PyObject* pyo);

SPI_PY_IMPORT
InputValues pyGetInputValues(
    FunctionCaller* func,
    PyObject* args,
    PyObject* self=0);

SPI_PY_IMPORT
std::vector<Value> pyTupleToValueVector(
    const char* name,
    size_t nbArgs,
    PyObject* args);

// to be used as part of the grand scheme of allowing Python to sub-class
// certain object types
//
// each method in the class will translate its inputs to a vector of values
// and then call this function to convert these into PyTuple which we can
// then pass into the PyCFunction once we have extracted it from the the
// PyObject
SPI_PY_IMPORT
PyObjectSP pyTupleFromValueVector(
    const std::vector<Value>& values);

SPI_END_NAMESPACE

#endif
