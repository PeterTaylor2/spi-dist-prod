#ifndef SPI_UTIL_JSON_VALUE_HPP
#define SPI_UTIL_JSON_VALUE_HPP

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
** JSONValue.hpp
***************************************************************************
** Defines various values that will be understood by JSON.
***************************************************************************
*/

#include "DeclSpec.h"
#include "Namespace.hpp"
#include "RefCounter.hpp"

#include <string>
#include <map>
#include <vector>

SPI_UTIL_NAMESPACE

class JSONValue;
SPI_UTIL_DECLARE_RC_CLASS(JSONString);
SPI_UTIL_DECLARE_RC_CLASS(JSONArray);
SPI_UTIL_DECLARE_RC_CLASS(JSONMap);

class SPI_UTIL_IMPORT JSONString : public RefCounter
{
public:
    friend class JSONValue;

    JSONString(const std::string& str);
    JSONString(const char* str);
    const std::string& String() const;

private:
    std::string m_string;
};


class SPI_UTIL_IMPORT JSONArray : public RefCounter
{
public:
    friend class JSONValue;

    JSONArray();
    void Append(const JSONValue& value);
    size_t Size() const;
    const JSONValue& Item(size_t i) const;

private:
    std::vector<JSONValue> m_array;

};

class SPI_UTIL_IMPORT JSONMap : public RefCounter
{
public:
    friend class JSONValue;

    JSONMap();

    const std::vector<std::string>& Names() const;
    const JSONValue& Item(const std::string& key) const;
    void Insert(const std::string& key, const JSONValue& value);

private:

    std::map<std::string, JSONValue> m_map;
    std::vector<std::string> m_names;

};

class SPI_UTIL_IMPORT JSONValue
{
public:
    enum Type
    {
        Number,
        String,
        Array,
        Map,
        Bool,
        Null
    };

    JSONValue();
    JSONValue(double);
    JSONValue(const std::string&);
    JSONValue(const JSONArraySP&);
    JSONValue(const JSONArrayConstSP&);
    JSONValue(const JSONMapSP&);
    JSONValue(const JSONMapConstSP&);
    JSONValue(bool);

    Type GetType() const;

    double             GetNumber() const;
    const std::string& GetString() const;
    JSONArrayConstSP   GetArray() const;
    JSONMapConstSP     GetMap() const;
    bool               GetBool() const;

    ~JSONValue();
    JSONValue(const JSONValue&);
    JSONValue& operator=(const JSONValue&);
    JSONValue& Swap(JSONValue&);

private:

    Type type;
    union
    {
        double            aNumber;
        const JSONString* aString;
        const JSONArray*  anArray;
        const JSONMap*    aMap;
        bool              aBool;
    };

};

SPI_UTIL_END_NAMESPACE

#endif
