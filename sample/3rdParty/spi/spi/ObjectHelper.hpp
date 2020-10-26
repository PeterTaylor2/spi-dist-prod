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
#ifndef SPI_OBJECT_HELPER_HPP
#define SPI_OBJECT_HELPER_HPP

/*
 * For a concrete class which is not a wrapper class you can use
 * SPI_DECLARE_OBJECT_TYPE(T) inside the public section of the class
 * declaration.
 *
 * In the object implementation file, define to_map and object_from_map
 * methods for class T, and also use SPI_IMPLEMENT_OBJECT_TYPE(T).
 *
 * In other words four things are needed for each concrete class derived
 * from Object.
 */

#include "Object.hpp"
#include "Value.hpp"
#include "ValueToObject.hpp"
#include "MatrixData.hpp"
#include "ObjectSmartPtr.hpp"

// NOTE: in the future we may declare outer_type independently
// The purpose would be to allow non-const definitions for outer_type
#define SPI_DECLARE_OBJECT_TYPE(T)\
    typedef spi::ObjectSmartPtr<T> outer_type;\
    spi::ObjectType* get_object_type() const;\
    void to_map(spi::IObjectMap* objMap, bool publicOnly) const;\
    static spi::ObjectConstSP object_from_map(spi::IObjectMap* objMap, spi::ValueToObject& mapToObject);\
    static spi::ObjectConstSP coerce_from_value(const spi::Value& v, const spi::InputContext* c);\
    static spi::ObjectConstSP coerce_from_object(const spi::ObjectConstSP& o);\
    static T::outer_type Coerce(const spi::ObjectConstSP& o);\
    static T::outer_type from_stream(std::istream& istr, const std::string& streamName, bool isBinary=false);\
    static T::outer_type from_string(const std::string& str);\
    static T::outer_type from_file(const std::string& filename);\
    template<typename U>\
    static std::vector<outer_type> CoerceVector(const std::vector<U>& inp)\
    {\
        size_t size = inp.size();\
        std::vector<outer_type> out;\
        for (size_t i = 0; i < size; ++i)\
            out.push_back(Coerce(inp[i]));\
        return out;\
    }\
    template<typename U>\
    static spi::MatrixData<outer_type> CoerceMatrix(const spi::MatrixData<U>& inp)\
    {\
        return spi::MatrixData<outer_type>(\
            inp.Rows(), inp.Cols(), CoerceVector(inp.Data()));\
    }\
    template<typename U>\
    static std::vector<outer_type> Wrap(const std::vector<U>& inp)\
    {\
        std::vector<outer_type> out;\
        out.reserve(inp.size());\
        for (typename std::vector<U>::const_iterator iter = inp.begin(); iter != inp.end(); ++iter)\
            out.push_back(Wrap(*iter));\
        return out;\
    }\
    static spi::ObjectType object_type

#define SPI_IMPLEMENT_OBJECT_TYPE(T,objectName,serviceFunc,canPut,coerceFrom)\
spi::ObjectType* T::get_object_type() const\
{\
    return &T::object_type;\
}\
T::outer_type T::from_stream(std::istream& istr, const std::string& streamName, bool isBinary)\
{\
    spi::ObjectConstSP o = serviceFunc()->object_from_stream(istr, streamName, isBinary);\
    return T::Coerce(o);\
}\
T::outer_type T::from_string(const std::string& str)\
{\
    spi::ObjectConstSP o = serviceFunc()->object_from_string(str);\
    return T::Coerce(o);\
}\
T::outer_type T::from_file(const std::string& filename)\
{\
    spi::ObjectConstSP o = serviceFunc()->object_from_file(filename);\
    return T::Coerce(o);\
}\
spi::ObjectConstSP T::coerce_from_object(const spi::ObjectConstSP& o)\
{\
    return (spi::ObjectConstSP)(T::Coerce(o));\
}\
spi::ObjectType T::object_type = spi::ObjectType(\
    objectName, T::object_from_map, spi::IsInstance<T>, canPut, T::coerce_from_object, coerceFrom)

SPI_BEGIN_NAMESPACE

// Safely (dynamic) cast a generic object shared pointer to a specific type
template<class T>
typename T::outer_type DynamicCast(const spi::ObjectConstSP& o)
{
    // isNull
    if (!o)
        return typename T::outer_type();

    // isInstance
    const T* p = dynamic_cast<const T*>(o.get());
    if (p)
        return typename T::outer_type(p);

    spi::ObjectType* ot = &T::object_type;

    SPI_THROW_RUNTIME_ERROR(o->get_class_name()
        << " is neither " << ot->get_class_name()
        << " nor a sub-class of " << ot->get_class_name());
}

// invokes the supplied copy function
template<typename T>
T* SafeCopy(T* in, T* (CopyFunc)(T*))
{
    if (!in)
        return NULL;
    T* out = CopyFunc(in);
    if (!out)
        throw RuntimeError("Copy failed");
    return out;
}

// invokes the class copy constructor
template<class T>
T* SafeCopy(T* in)
{
    if (!in)
        return NULL;
    T* out = new T(*in);
    return out;
}

// gets a public attribute from an object

SPI_IMPORT
Value ObjectGet(const ObjectConstSP& obj, const char* name);

SPI_IMPORT
Value ObjectGet(const Object* obj, const char* name);

class SPI_IMPORT ObjectHelperCache
{
public:
    ObjectHelperCache(const char* name=NULL);
    ~ObjectHelperCache();

    Object* Find(const void* inner) const;
    void Insert(const void* inner, const Object* outer);
    void Delete(const void* inner, const Object* outer);
    size_t Size() const;

private:
    const char* m_name;
    std::map<const void*, const Object*> m_map;

    ObjectHelperCache(const ObjectHelperCache&);
    ObjectHelperCache& operator =(const ObjectHelperCache&);
};

SPI_END_NAMESPACE


#endif
