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
#ifndef SPI_PY_OBJECTMAP_HPP
#define SPI_PY_OBJECTMAP_HPP

/*
***************************************************************************
** pyObjectMap.hpp
**
** Defines mapping between a python dictionary and an IObjectMap interface.
***************************************************************************
*/

#include "pyObject.h"

#include "pyUtil.hpp"
#include "pyValue.hpp"
#include "pyOutput.hpp"

#include "../Value.hpp"
#include "../Object.hpp"
#include "../IObjectMap.hpp"

SPI_BEGIN_NAMESPACE

void pyDictToObjectMap(
    PyObject* dict,
    IObjectMap* objectMap);

SPI_PY_IMPORT
PyObjectSP pyObjectMapToDict(
    IObjectMap* objectMap,
    ValueToObject& valueToObject);

SPI_PY_IMPORT
void pyObjectToMap(
    PyObject* self,
    IObjectMap* objectMap,
    bool publicOnly);

SPI_END_NAMESPACE

#endif
