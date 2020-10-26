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
** ValueToObject.hpp
***************************************************************************
** Class for converting values to objects.
***************************************************************************
*/

#ifndef SPI_VALUE_TO_OBJECT_HPP
#define SPI_VALUE_TO_OBJECT_HPP

#include "Object.hpp"

#include <map>
#include <istream>
#include <ostream>

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(Service);
SPI_DECLARE_RC_CLASS(ObjectRefCache);

class Value;

/**
 * Provides the information to help with converting a map to an object.
 *
 * The issues are that the we need to find the object constructor (for
 * which we use the service), and that we need to be able to get from
 * an object reference to an object for objects we have seen before.
 */
class SPI_IMPORT ValueToObject
{
public:
    ValueToObject(
        const ServiceConstSP& svc,
        const ObjectRefCacheSP& objectCache);
    ~ValueToObject();

    ObjectConstSP ObjectFromValue(
        const Value& value,
        ObjectType* objectType);

    // converts a Value by resolving maps and references to objects
    Value ResolveObject(const Value& value);
    ServiceConstSP Service() const;

private:
    ServiceConstSP   m_service;
    ObjectRefCacheSP m_objectCache;

    ValueToObject(const ValueToObject&);
    ValueToObject& operator=(const ValueToObject&);
};

SPI_END_NAMESPACE

#endif
