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
** pyInput.hpp
**
** Functions with convert from the Python data type (PyObject) to various
** standard and spi-specific c++ data types.
**
** Each function has a scalar and a vector equivalent.
**
** The vector version will convert the input to PyObject to a Python sequence
** (if possible) and then call the scalar equivalent on each element of the
** sequence.
**
** Note that all functions are in the spi namespace, but also use the
** prefix py (lower case). This is to distinguish them from the Python
** C-interface which uses the prefix Py (mixed case). Otherwise there
** might be name clashes which would require us to use :: before functions
** from the Python C-interface.
***************************************************************************
*/

#ifndef SPI_PYINPUT_HPP
#define SPI_PYINPUT_HPP

#include "Python.h"

#include "pyUtil.hpp"

#include <spi/Date.hpp>
#include <spi/Object.hpp>
#include <spi/Value.hpp>
#include <spi/Map.hpp>

#include <string>
#include <vector>

SPI_BEGIN_NAMESPACE

/**
 * Converts from PyObject to bool.
 *
 * Accepts False, True and integers.
 */
bool pyoToBool(PyObject* pyo);
std::vector<bool> pyoToBoolVector(PyObject* pyo);

/**
 * Converts from PyObject to int.
 *
 * We do the equivalent of the Python int function, and then validate that
 * resulting integer is a valid C int (since Python integers are represented
 * by long in C).
 */
int pyoToInt(PyObject* pyo);
std::vector<int> pyoToIntVector(PyObject* pyo);

/**
 * Converts from PyObject to long.
 *
 * We do the equivalent of the Python int function. The resulting integer
 * will be a long and is returned.
 */
long pyoToLong(PyObject* pyo);
std::vector<long> pyoToLongVector(PyObject* pyo);

/**
 * Converts from PyObject to size_t.
 *
 * We do the equivalent of the Python int function, and then validate that
 * resulting integer is positive.
 */
size_t pyoToSizeT(PyObject* pyo);
std::vector<size_t> pyoToSizeTVector(PyObject* pyo);

/**
 * Converts from PyObject to double.
 *
 * We do the equivalent of the Python float function. Since Python represents
 * float as double in C, the resulting value is simply returned.
 */
double pyoToDouble(PyObject* pyo);
std::vector<double> pyoToDoubleVector(PyObject* pyo);

/**
 * Converts from PyObject to std::string.
 *
 * The input must be a Python string.
 */
bool pyoIsString(PyObject* pyo);
std::string pyoToString(PyObject* pyo);
std::vector<std::string> pyoToStringVector(PyObject* pyo);

/**
 * Converts from PyObject to char.
 *
 * The input must be a Python string of length 1.
 */
char pyoToChar(PyObject* pyo);
std::vector<char> pyoToCharVector(PyObject* pyo);

/**
 * Converts from PyObject to Date.
 *
 * Accepts datetime.date as input, and also integers of the format YYYYMMDD.
 */
Date pyoToDate(PyObject* pyo);
std::vector<Date> pyoToDateVector(PyObject* pyo);

/**
 * Converts from PyObject to ObjectSP.
 *
 * We provide the object type so that we can validate that we have the correct
 * class, and in the future to provide object coercion.
 *
 * Returns None as an empty object.
 */
ObjectConstSP pyoToObject(PyObject* pyo, ObjectType* ot);
std::vector<ObjectConstSP> pyoToObjectVector(PyObject* pyo, ObjectType* ot=0);

#if 0
template<class T>
spi_boost::intrusive_ptr<T> pyoToInstance(
    PyObject* pyo)
{
    ObjectConstSP obj = pyoToConstObject(pyo, &T::object_type);
    return CoerceFromObject<T>(obj);
}

template<class T>
std::vector< spi_boost::intrusive_ptr<T> > pyoToInstanceVector(
    PyObject* pyo)
{
    std::vector<ObjectConstSP> tmp = pyoToObjectVector(pyo);
    std::vector<spi_boost::intrusive_ptr<T> > output;
    output.reserve(tmp.size());
    for (size_t i = 0; i < tmp.size(); ++i)
        output.push_back(CoerceFromObject<T>(tmp[i]));
    return output;
}
#endif

SPI_END_NAMESPACE

#endif
