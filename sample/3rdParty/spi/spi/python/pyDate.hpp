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
** pyDate.hpp
**
** Primitive Python date conversion routines.
**
** Only this module accesses the Python DateTime module.
***************************************************************************
*/

#ifndef SPI_PYDATE_HPP
#define SPI_PYDATE_HPP

#include "Python.h"

#include <spi/Date.hpp>
#include <spi/DateTime.hpp>

SPI_BEGIN_NAMESPACE

bool pyIsDate(PyObject* pyo);
bool pyIsDateTime(PyObject* pyo);
PyObject* pyMakeDate(int year, int month, int day);
PyObject* pyMakeDateTime(int year, int month, int day,
    int hours, int minutes, int seconds);
Date pyToDate(PyObject* pyo);
DateTime pyToDateTime(PyObject* pyo);

SPI_END_NAMESPACE

#endif
