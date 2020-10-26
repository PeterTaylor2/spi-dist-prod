#ifndef CPPLIB_RESULTS_HPP
#define CPPLIB_RESULTS_HPP

/*

Copyright (C) 2015 Sartorial Programming Ltd.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

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
** Results.hpp
***************************************************************************
** Defines types that can be used to define generic results.
**
** Not really intended for input to the library.
**
** Typically your function might return CMap which contains any many of
** analytics results calculated by that function.
***************************************************************************
*/

#include "RefCounter.hpp"
#include "Date.hpp"
#include "RuntimeError.hpp"

#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <string.h>

class CValue;

CPPLIB_DECLARE_RC_CLASS(CString);
CPPLIB_DECLARE_RC_CLASS(CArray);
CPPLIB_DECLARE_RC_CLASS(CMap);
CPPLIB_DECLARE_RC_CLASS(CPointer);

class CString : public CRefCounter
{
public:
    friend class CValue;

    CString(const std::string& str);
    CString(const char* str);
    const std::string& String() const;

private:
    std::string m_string;
};

class CArray : public CRefCounter
{
public:
    friend class CValue;

    CArray();
    void Append(const CValue& value);
    size_t Size() const;
    const CValue& Item(size_t i) const;

private:
    std::vector<CValue> m_array;
};

class CMap : public CRefCounter
{
public:
    friend class CValue;

    CMap();

    const std::vector<std::string>& Names() const;
    const CValue& Item(const std::string& key) const;
    void Insert(const std::string& key, const CValue& value);

private:

    std::map<std::string, CValue> m_map;
    std::vector<std::string> m_names;
};

class CPointer : public CRefCounter
{
public:
    friend class CValue;

    CPointer(const char* typeName, 
        const boost::shared_ptr<const void>& sp);

    template<class T>
    CPointer(const boost::shared_ptr<const T>& sp)
        :
        m_typeName(T::TypeName),
        m_sp(sp)
    {}

    template<class T>
    boost::shared_ptr<const T> SP() const
    {
        if (strcmp(T::TypeName, m_typeName) != 0)
        {
            THROW_RUNTIME_ERROR("Cannot convert '" << m_typeName << "' to '" 
                << T::TypeName << "'");
        }

        // the TypeNames match - so static_pointer_cast should be OK (we hope)
        return boost::static_pointer_cast<const T>(m_sp);
    }

    const char* TypeName() const;
    const boost::shared_ptr<const void>& SP() const;

private:
    const char* m_typeName;
    boost::shared_ptr<const void> m_sp;
};

class CValue
{
public:
    enum Type
    {
        Double,
        Integer,
        Date,
        String,
        Array,
        Map,
        Bool,
        Pointer,
        Null
    };

    CValue();
    CValue(double);
    CValue(int);
    CValue(CDate);
    CValue(const std::string&);
    CValue(const CArraySP&);
    CValue(const CArrayConstSP&);
    CValue(const CMapSP&);
    CValue(const CMapConstSP&);
    CValue(bool);
    CValue(const CPointerSP&);
    CValue(const CPointerConstSP&);

    template<class T>
    CValue(const boost::shared_ptr<const T>& sp)
        :
        type(Pointer),
        aPointer(NULL)
    {
        aPointer = new CPointer(sp);
        incRefCount(aPointer);
    }
    
    Type GetType() const;

    double             GetDouble() const;
    int                GetInt() const;
    CDate              GetDate() const;
    const std::string& GetString() const;
    CArrayConstSP      GetArray() const;
    CMapConstSP        GetMap() const;
    bool               GetBool() const;
    CPointerConstSP    GetPointer() const;
    
    template<class T>
    boost::shared_ptr<const T> GetPointer() const
    {
        if (type == Pointer)
        {
            if (!aPointer)
                return boost::shared_ptr<const T>();
            return aPointer->SP<T>();
        }

        THROW_RUNTIME_ERROR("Value is not a pointer");
    }

    operator double() const
    {
        return GetDouble();
    }

    operator int() const
    {
        return GetInt();
    }

    operator CDate() const
    {
        return GetDate();
    }

    operator const std::string&() const
    {
        return GetString();
    }

    operator std::string() const
    {
        return GetString();
    }

    operator bool() const
    {
        return GetBool();
    }

    template<class T>
    operator boost::shared_ptr<const T> () const
    {
        return GetPointer<T>();
    }

    ~CValue();
    CValue(const CValue&);
    CValue& operator=(const CValue&);
    CValue& Swap(CValue&);

private:

    Type type;
    union
    {
        double          aDouble;
        int             anInt;
        int             aDate;
        const CString*  aString;
        const CArray*   anArray;
        const CMap*     aMap;
        const CPointer* aPointer;
        bool            aBool;
    };

};

#endif
