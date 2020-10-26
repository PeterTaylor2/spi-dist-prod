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
** pyObject.h
**
** Python object equivalent to spi::Object
***************************************************************************
*/

#ifndef SPI_PYOBJECT_H
#define SPI_PYOBJECT_H

#include "Python.h"

#ifndef __cplusplus

typedef struct _SpiObject SpiObject;

#else

#include "pyUtil.hpp"
typedef spi::Object SpiObject;

extern "C"
{
#endif

    typedef struct _SpiPyObject
    {
        PyObject_HEAD
        const SpiObject* obj;
    } SpiPyObject;

    SPI_PY_IMPORT
    void spi_py_object_dealloc (SpiPyObject *pyo);

    SPI_PY_IMPORT
    PyObject* spi_py_object_getter (SpiPyObject* pyo, char* name);

    SPI_PY_IMPORT
    int spi_py_object_setter (SpiPyObject* pyo, PyObject* value, char* name);

    SPI_PY_IMPORT
    int spi_py_object_map_setter(SpiPyObject* pyo, PyObject* value, char* name);

    SPI_PY_IMPORT
    PyObject* spi_py_object_getattro(SpiPyObject* pyo, PyObject* name);

    SPI_PY_IMPORT
    PyObject* spi_py_object_get(PyObject* self, PyObject* args);

    SPI_PY_IMPORT
    PyObject* spi_py_object_put(PyObject* self, PyObject* args);

    SPI_PY_IMPORT
    PyObject* spi_py_object_to_string (PyObject* pyo, PyObject* args);

    SPI_PY_IMPORT
    PyObject* spi_py_object_to_file (PyObject* pyo, PyObject* args);

    SPI_PY_IMPORT
    PyObject* spi_py_object_from_string (PyObject* self, PyObject* args);

    SPI_PY_IMPORT
    PyObject* spi_py_object_from_file (PyObject* self, PyObject* args);

    PyTypeObject* SpiPyObjectType();

#ifdef __cplusplus
}
#endif

#endif

