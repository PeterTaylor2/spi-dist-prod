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
/**
 * Mechanism for translating an enum to/from strings.
 *
 * The EnumLookup provides a two-way mapping from integer to string.
 * From string to integer provides case-indepedent lookup.
 * From integer to string chooses the first available string for that integer.
 * You can provide multiple strings for each integer, but the string must be
 * unique.
 */

#include "EnumLookup.hpp"
#include "RuntimeError.hpp"

#include <spi_util/StringUtil.hpp>

CEnumLookup::CEnumLookup(const char* name)
    :
    m_name(name),
    m_indexStringCode(),
    m_indexCodeString()
{
    if (!name)
        THROW_RUNTIME_ERROR("Undefined name");
}

void CEnumLookup::AddItem(int code, const char* str)
{
    PRE_CONDITION(str != NULL);

    const std::string& uc = spi_util::CStringUpper(str);
    
    if (m_indexStringCode.count(uc) != 0)
        THROW_RUNTIME_ERROR("Duplicate string '" << str << "' provided for '" 
                            << m_name << "'");

    m_indexStringCode[uc] = code;

    if (m_indexCodeString.count(code) == 0)
        m_indexCodeString[code] = str;
}

int CEnumLookup::FromString(const char* str) const
{
    PRE_CONDITION(str != NULL);

    std::map<std::string, int>::const_iterator iter =
        m_indexStringCode.find(spi_util::CStringUpper(str));

    if (iter == m_indexStringCode.end())
        THROW_RUNTIME_ERROR("Undefined string '" << str << "' for '" << m_name << "'");

    return iter->second;
}
 
const std::string& CEnumLookup::ToString(int code) const
{
    std::map<int, std::string>::const_iterator iter =
        m_indexCodeString.find(code);

    if (iter == m_indexCodeString.end())
        THROW_RUNTIME_ERROR("Undefined code " << code << " for '" << m_name << "'");

    return iter->second;
}

bool CEnumLookup::IsValid(int code) const
{
    std::map<int, std::string>::const_iterator iter =
        m_indexCodeString.find(code);

    return iter != m_indexCodeString.end();
}
