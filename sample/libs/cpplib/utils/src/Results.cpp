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

#include "Results.hpp"

#include "RuntimeError.hpp"
#include <string.h>

/*
***************************************************************************
** Implementation of CString
***************************************************************************
*/
CString::CString(const std::string& str)
    :
    m_string(str)
{}

CString::CString(const char* str)
    :
    m_string(str ? str : "")
{}

const std::string& CString::String() const
{
    return m_string;
}

/*
***************************************************************************
** Implementation of CArray
***************************************************************************
*/
CArray::CArray()
    :
    m_array()
{}

void CArray::Append(const CValue& value)
{
    m_array.push_back(value);
}

size_t CArray::Size() const
{
    return m_array.size();
}

const CValue& CArray::Item(size_t i) const
{
    if (i > m_array.size())
        THROW_RUNTIME_ERROR("CArray: Item out of range");

    return m_array[i];
}

/*
***************************************************************************
** Implementation of CMap
***************************************************************************
*/
CMap::CMap()
{}

const std::vector<std::string>& CMap::Names() const
{
    return m_names;
}

const CValue& CMap::Item(const std::string& key) const
{
    std::map<std::string, CValue>::const_iterator iter = m_map.find(key);

    if (iter == m_map.end())
        THROW_RUNTIME_ERROR("CMap: Item undefined");

    return iter->second;
}

void CMap::Insert(const std::string& key, const CValue& value)
{
    if (m_map.count(key) == 0)
        m_names.push_back(key);

    m_map[key] = value;
}

/*
***************************************************************************
** Implementation of CPointer
***************************************************************************
*/
const char* CPointer::TypeName() const
{
    return m_typeName;
}

CPointer::CPointer(const char* typeName, 
    const boost::shared_ptr<const void>& sp)
    :
    m_typeName(typeName), m_sp(sp)
{}

const boost::shared_ptr<const void>& CPointer::SP() const
{
    return m_sp;
}

/*
***************************************************************************
** Implementation of CValue
***************************************************************************
*/
CValue::CValue()
    :
    type(CValue::Null),
    aDouble(0.0)
{}

CValue::CValue(double number)
    :
    type(CValue::Double),
    aDouble(number)
{}

CValue::CValue(int number)
    :
    type(CValue::Integer),
    anInt(number)
{}

CValue::CValue(CDate date)
    :
    type(CValue::Date),
    aDate(int(date))
{}

CValue::CValue(const std::string& str)
    :
    type(CValue::String),
    aDouble(0.0)
{
    aString = new CString(str);
    incRefCount(aString);
}

CValue::CValue(const CArraySP& in)
    :
    type(CValue::Array),
    aDouble(0.0)
{
    anArray = in.get();
    incRefCount(anArray);
}

CValue::CValue(const CArrayConstSP& in)
    :
    type(CValue::Array),
    aDouble(0.0)
{
    anArray = in.get();
    incRefCount(anArray);
}

CValue::CValue(const CMapSP& in)
    :
    type(CValue::Map),
    aDouble(0.0)
{
    aMap = in.get();
    incRefCount(aMap);
}

CValue::CValue(const CMapConstSP& in)
    :
    type(CValue::Map),
    aDouble(0.0)
{
    aMap = in.get();
    incRefCount(aMap);
}

CValue::CValue(bool in)
    :
    type(CValue::Bool),
    aBool(in)
{}

CValue::Type CValue::GetType() const
{
    return type;
}

double CValue::GetDouble() const
{
    if (type == Double)
        return aDouble;

    THROW_RUNTIME_ERROR("Value is not a double");
}

int CValue::GetInt() const
{
    if (type == Integer)
        return anInt;

    THROW_RUNTIME_ERROR("Value is not an integer");
}

CDate CValue::GetDate() const
{
    if (type == Date)
        return CDate(aDate);

    THROW_RUNTIME_ERROR("Value is not a date");
}

const std::string& CValue::GetString() const
{
    if (type == String)
        return aString->String();

    THROW_RUNTIME_ERROR("Value is not a string");
}

CArrayConstSP CValue::GetArray() const
{
    if (type == Array)
        return CArrayConstSP(anArray);

    THROW_RUNTIME_ERROR("Value is not an array");
}

CMapConstSP CValue::GetMap() const
{
    if (type == Map)
        return CMapConstSP(aMap);

    THROW_RUNTIME_ERROR("Value is not a map");
}

bool CValue::GetBool() const
{
    if (type == Bool)
        return aBool;

    THROW_RUNTIME_ERROR("Value is not a bool");
}

CPointerConstSP CValue::GetPointer() const
{
    if (type == Pointer)
        return aPointer;

    THROW_RUNTIME_ERROR("Value is not a pointer");
}

CValue::~CValue()
{
    switch(type)
    {
    case String:
        decRefCount(aString);
        break;
    case Array:
        decRefCount(anArray);
        break;
    case Map:
        decRefCount(aMap);
        break;
    case Pointer:
        decRefCount(aPointer);
        break;
    case Double:
    case Integer:
    case Date:
    case Bool:
    case Null:
        break;
    }

    type = Null;
}

CValue::CValue(const CValue& in)
    :
    type(in.type),
    aDouble(0.0)
{
    switch(type)
    {
    case String:
        aString = in.aString;
        incRefCount(aString);
        break;
    case Array:
        anArray = in.anArray;
        incRefCount(anArray);
        break;
    case Map:
        aMap = in.aMap;
        incRefCount(aMap);
        break;
    case Pointer:
        aPointer = in.aPointer;
        incRefCount(aPointer);
        break;
    case Double:
        aDouble = in.aDouble;
        break;
    case Integer:
        anInt = in.anInt;
        break;
    case Bool:
        aBool = in.aBool;
        break;
    case Date:
        aDate = in.aDate;
        break;
    case Null:
        break;
    }
}

CValue& CValue::operator=(const CValue& rhs)
{
    CValue tmp(rhs);

    Swap(tmp); // bitwise copy

    return *this;
}

CValue& CValue::Swap(CValue &value)
{
    // Simple bitwise swap
    char tmp[sizeof(CValue)];
    memcpy(tmp, this, sizeof(CValue));
    memcpy(this, &value, sizeof(CValue));
    memcpy(&value, tmp, sizeof(CValue));
    return *this;
}

