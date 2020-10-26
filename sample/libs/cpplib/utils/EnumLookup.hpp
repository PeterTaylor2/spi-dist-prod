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
#ifndef CPPLIB_ENUM_LOOKUP_HPP
#define CPPLIB_ENUM_LOOKUP_HPP

/**
 * Mechanism for translating an enum to/from strings.
 *
 * The EnumLookup provides a two-way mapping from integer to string.
 * From string to integer provides case-indepedent lookup.
 * From integer to string chooses the first available string for that integer.
 * You can provide multiple strings for each integer, but the string must be
 * unique.
 */

#include <string>
#include <map>

class CEnumLookup
{
public:
    CEnumLookup(const char* name);
    void AddItem(int code, const char* str);
    int FromString(const char* str) const;
    const std::string& ToString(int code) const;
    bool IsValid(int code) const;

private:
    const char* m_name;
    std::map<std::string, int> m_indexStringCode;
    std::map<int, std::string> m_indexCodeString;
};

#endif

