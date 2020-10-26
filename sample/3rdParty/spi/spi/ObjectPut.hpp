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
** ObjectPut.hpp
***************************************************************************
** Defines functionality for ObjectPut.
**
** Two use modes:
** 1. Modifying an object of known type.
** 2. Creating a pure map without a known type.
**
** Start with the first use mode - we create a sub-class of IObjectMap
** which contains two maps - the map of the object to be modified plus an
** in context map of the changes.
**
** Second use case will have an in context map of the changes, but you will
** be expected to give clues as to the data types by your choice of names.
***************************************************************************
*/

#ifndef SPI_OBJECT_PUT_HPP
#define SPI_OBJECT_PUT_HPP

#include "Object.hpp"
#include "ObjectHelper.hpp"
#include "Value.hpp"

#include <string>
#include <vector>

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(Map);
SPI_DECLARE_RC_CLASS(MapObject);

SPI_IMPORT
ObjectConstSP ObjectPut(
    const ObjectConstSP& obj,
    const char* name,
    const Value& value,
    const InputContext*             context);

ObjectConstSP ObjectPut(
    const ObjectConstSP& obj,
    const std::vector<std::string>& names,
    const std::vector<Value>&       values,
    const InputContext*             context);

//class MapObject : public Object
//{
//public:
//    MapObject(
//        const std::vector<std::string>& names,
//        const std::vector<Value>&       values,
//        const InputContext*             context);
//
//    SPI_DECLARE_OBJECT_TYPE(MapObject);
//
//private:
//    MapSP m_map;
//};

SPI_END_NAMESPACE

#endif
