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
#ifndef CPPLIB_RUNTIME_ERROR_HPP
#define CPPLIB_RUNTIME_ERROR_HPP

/*
***************************************************************************
** Various macros for generating std::runtime_error.
***************************************************************************
*/

#include <sstream>
#include <stdexcept>
#include <vector>

#define THROW_RUNTIME_ERROR(msg) { \
std::ostringstream oss_;\
oss_ << __FUNCTION__ << ": " << msg;\
throw std::runtime_error(oss_.str());\
}

#undef PRE_CONDITION
#define PRE_CONDITION(cond) do { if(!(cond)) {\
THROW_RUNTIME_ERROR("Required pre-condition (" << #cond << ") fails"); \
}} while (0)

#undef POST_CONDITION
#define POST_CONDITION(cond) do { if(!(cond)) {\
THROW_RUNTIME_ERROR("Required post-condition (" << #cond << ") fails"); \
}} while (0)

#define NOT_IMPLEMENTED THROW_RUNTIME_ERROR("Not implemented")

#define RETHROW(e, msg) { \
std::ostringstream oss_;\
oss_ << e.what() << "\n" << __FUNCTION__ << ": " << msg;\
throw std::runtime_error(oss_.str());\
}

#define THROW_FUNCTION_FAILURE(e) RETHROW(e, "Failed.")

template<typename T, typename U>
void CheckArraySize(
    const std::vector<T>& v1,
    const std::vector<U>& v2,
    const char* nm1,
    const char* nm2)
{
    if (v1.size() != v2.size())
        THROW_RUNTIME_ERROR("Array size mismatch: " << nm1 << " (" << v1.size() << ") "
            << nm2 << " (" << v2.size() << ")");
}

#endif /* CPPLIB_RUNTIME_ERROR_HPP */

