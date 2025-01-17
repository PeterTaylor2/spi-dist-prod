#ifndef SPI_BOOST_SMART_PTR_DETAIL_LWM_WIN32_CS_HPP_INCLUDED
#define SPI_BOOST_SMART_PTR_DETAIL_LWM_WIN32_CS_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  spi_boost/detail/lwm_win32_cs.hpp
//
//  Copyright (c) 2002, 2003 Peter Dimov
//  Copyright (c) Microsoft Corporation 2014
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <spi_boost/predef.h>

#ifdef SPI_BOOST_USE_WINDOWS_H

#include <windows.h>

#else

struct _RTL_CRITICAL_SECTION;

#endif

namespace spi_boost
{

namespace detail
{

#ifndef SPI_BOOST_USE_WINDOWS_H

struct critical_section
{
    struct critical_section_debug * DebugInfo;
    long LockCount;
    long RecursionCount;
    void * OwningThread;
    void * LockSemaphore;
#if defined(_WIN64)
    unsigned __int64 SpinCount;
#else
    unsigned long SpinCount;
#endif
};

#if SPI_BOOST_PLAT_WINDOWS_RUNTIME
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSectionEx(::_RTL_CRITICAL_SECTION *, unsigned long, unsigned long);
#else
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(::_RTL_CRITICAL_SECTION *);
#endif
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(::_RTL_CRITICAL_SECTION *);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(::_RTL_CRITICAL_SECTION *);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(::_RTL_CRITICAL_SECTION *);

typedef ::_RTL_CRITICAL_SECTION rtl_critical_section;

#else // #ifndef SPI_BOOST_USE_WINDOWS_H

typedef ::CRITICAL_SECTION critical_section;

#if SPI_BOOST_PLAT_WINDOWS_RUNTIME
using ::InitializeCriticalSectionEx;
#else
using ::InitializeCriticalSection;
#endif
using ::EnterCriticalSection;
using ::LeaveCriticalSection;
using ::DeleteCriticalSection;

typedef ::CRITICAL_SECTION rtl_critical_section;

#endif // #ifndef SPI_BOOST_USE_WINDOWS_H

class lightweight_mutex
{
private:

    critical_section cs_;

    lightweight_mutex(lightweight_mutex const &);
    lightweight_mutex & operator=(lightweight_mutex const &);

public:

    lightweight_mutex()
    {
#if SPI_BOOST_PLAT_WINDOWS_RUNTIME
        spi_boost::detail::InitializeCriticalSectionEx(reinterpret_cast< rtl_critical_section* >(&cs_), 4000, 0);
#else
        spi_boost::detail::InitializeCriticalSection(reinterpret_cast< rtl_critical_section* >(&cs_));
#endif
    }

    ~lightweight_mutex()
    {
        spi_boost::detail::DeleteCriticalSection(reinterpret_cast< rtl_critical_section* >(&cs_));
    }

    class scoped_lock;
    friend class scoped_lock;

    class scoped_lock
    {
    private:

        lightweight_mutex & m_;

        scoped_lock(scoped_lock const &);
        scoped_lock & operator=(scoped_lock const &);

    public:

        explicit scoped_lock(lightweight_mutex & m): m_(m)
        {
            spi_boost::detail::EnterCriticalSection(reinterpret_cast< rtl_critical_section* >(&m_.cs_));
        }

        ~scoped_lock()
        {
            spi_boost::detail::LeaveCriticalSection(reinterpret_cast< rtl_critical_section* >(&m_.cs_));
        }
    };
};

} // namespace detail

} // namespace spi_boost

#endif // #ifndef SPI_BOOST_SMART_PTR_DETAIL_LWM_WIN32_CS_HPP_INCLUDED
