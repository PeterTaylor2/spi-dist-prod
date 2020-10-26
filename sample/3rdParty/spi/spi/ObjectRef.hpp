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
** ObjectRef.hpp
***************************************************************************
** Defines a reference to an object, plus the cache for mapping object
** references to objects.
**
** Value can be object reference, hence we need to be able to convert
** a reference to an object within the GetObject method in a Map. This
** requires us to put the cache somewhere.
**
** At present my thought is to put it into the Map, and when a Map is asked
** to create a new Map (which is how subsidiary maps are created when
** parsing an input stream) this cache is passed on (as a shared pointer
** of course) to the new Map.
***************************************************************************
*/

#ifndef SPI_OBJECT_REF_HPP
#define SPI_OBJECT_REF_HPP

#include "RefCounter.hpp"

#include <map>

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(Object);
SPI_DECLARE_RC_CLASS(ObjectRefCache);

class SPI_IMPORT ObjectRef
{
public:
    // constructor is declared as explicit to prevent automatic type conversion
    // hence when we construct a Value by automatic type conversion we won't
    // get ObjectRef by accident when we pass an integer
    explicit ObjectRef(int id);

    operator int() const;

private:
    int m_id;
};

class SPI_IMPORT ObjectRefCache : public RefCounter
{
public:
    ObjectRefCache();

    /**
     * Adds an object to the cache. Returns true if the object is already
     * in the cache, and hence you can probably just refer to that object
     * by its reference id.
     */
    bool add_object(int ref, const ObjectConstSP& obj);

    /**
     * Finds the object given the reference id. Returns a NULL object if the
     * reference id is not in the cache.
     */
    ObjectConstSP find_object(const ObjectRef& ref) const;
    ObjectConstSP find_object(int ref) const;

private:
    std::map<int, ObjectConstSP> m_dict;

    ObjectRefCache(const ObjectRefCache&);
    ObjectRefCache& operator=(const ObjectRefCache&);
};


SPI_END_NAMESPACE

#endif

