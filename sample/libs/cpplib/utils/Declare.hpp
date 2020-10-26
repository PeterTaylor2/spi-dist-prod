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
#ifndef CPPLIB_DECLARE_HPP
#define CPPLIB_DECLARE_HPP

#include <boost/shared_ptr.hpp>

#undef CPPLIB_DECLARE_CLASS
#define CPPLIB_DECLARE_CLASS(T) \
class T;\
typedef boost::shared_ptr<T>       T##SP;\
typedef boost::shared_ptr<T const> T##ConstSP

#undef CPPLIB_DECLARE_STRUCT
#define CPPLIB_DECLARE_STRUCT(T) \
struct T;\
typedef boost::shared_ptr<T>       T##SP;\
typedef boost::shared_ptr<T const> T##ConstSP

#undef CPPLIB_DECLARE_TYPEDEF_STRUCT
#define CPPLIB_DECLARE_TYPEDEF_STRUCT(T)  \
typedef struct _##T T;\
typedef boost::shared_ptr<T>       T##SP;\
typedef boost::shared_ptr<T const> T##ConstSP

template<class T>
boost::shared_ptr<T const> ShareConst(T* ptr)
{
    return boost::shared_ptr<T const>(ptr);
}

template<class T>
boost::shared_ptr<T> Share(T* ptr)
{
    return boost::shared_ptr<T>(ptr);
}

template<typename T>
boost::shared_ptr<T const> ShareConst(T* ptr, void (freeFunc)(T* ptr))
{
    return boost::shared_ptr<T const>(ptr, freeFunc);
}

template<typename T>
boost::shared_ptr<T> Share(T* ptr, void (freeFunc)(T* ptr))
{
    return boost::shared_ptr<T>(ptr, freeFunc);
}

#endif

