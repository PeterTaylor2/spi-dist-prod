/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SPI_BOOST_PREDEF_ARCHITECTURE_X86_32_H
#define SPI_BOOST_PREDEF_ARCHITECTURE_X86_32_H

#include <spi_boost/predef/version_number.h>
#include <spi_boost/predef/make.h>

/*`
[heading `SPI_BOOST_ARCH_X86_32`]

[@http://en.wikipedia.org/wiki/X86 Intel x86] architecture:
If available versions \[3-6\] are specifically detected.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`i386`] [__predef_detection__]]
    [[`__i386__`] [__predef_detection__]]
    [[`__i486__`] [__predef_detection__]]
    [[`__i586__`] [__predef_detection__]]
    [[`__i686__`] [__predef_detection__]]
    [[`__i386`] [__predef_detection__]]
    [[`_M_IX86`] [__predef_detection__]]
    [[`_X86_`] [__predef_detection__]]
    [[`__THW_INTEL__`] [__predef_detection__]]
    [[`__I86__`] [__predef_detection__]]
    [[`__INTEL__`] [__predef_detection__]]

    [[`__I86__`] [V.0.0]]
    [[`_M_IX86`] [V.0.0]]
    [[`__i686__`] [6.0.0]]
    [[`__i586__`] [5.0.0]]
    [[`__i486__`] [4.0.0]]
    [[`__i386__`] [3.0.0]]
    ]
 */

#define SPI_BOOST_ARCH_X86_32 SPI_BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(i386) || defined(__i386__) || \
    defined(__i486__) || defined(__i586__) || \
    defined(__i686__) || defined(__i386) || \
    defined(_M_IX86) || defined(_X86_) || \
    defined(__THW_INTEL__) || defined(__I86__) || \
    defined(__INTEL__)
#   undef SPI_BOOST_ARCH_X86_32
#   if !defined(SPI_BOOST_ARCH_X86_32) && defined(__I86__)
#       define SPI_BOOST_ARCH_X86_32 SPI_BOOST_VERSION_NUMBER(__I86__,0,0)
#   endif
#   if !defined(SPI_BOOST_ARCH_X86_32) && defined(_M_IX86)
#       define SPI_BOOST_ARCH_X86_32 SPI_BOOST_PREDEF_MAKE_10_VV00(_M_IX86)
#   endif
#   if !defined(SPI_BOOST_ARCH_X86_32) && defined(__i686__)
#       define SPI_BOOST_ARCH_X86_32 SPI_BOOST_VERSION_NUMBER(6,0,0)
#   endif
#   if !defined(SPI_BOOST_ARCH_X86_32) && defined(__i586__)
#       define SPI_BOOST_ARCH_X86_32 SPI_BOOST_VERSION_NUMBER(5,0,0)
#   endif
#   if !defined(SPI_BOOST_ARCH_X86_32) && defined(__i486__)
#       define SPI_BOOST_ARCH_X86_32 SPI_BOOST_VERSION_NUMBER(4,0,0)
#   endif
#   if !defined(SPI_BOOST_ARCH_X86_32) && defined(__i386__)
#       define SPI_BOOST_ARCH_X86_32 SPI_BOOST_VERSION_NUMBER(3,0,0)
#   endif
#   if !defined(SPI_BOOST_ARCH_X86_32)
#       define SPI_BOOST_ARCH_X86_32 SPI_BOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if SPI_BOOST_ARCH_X86_32
#   define SPI_BOOST_ARCH_X86_32_AVAILABLE
#endif

#define SPI_BOOST_ARCH_X86_32_NAME "Intel x86-32"

#include <spi_boost/predef/architecture/x86.h>

#endif

#include <spi_boost/predef/detail/test.h>
SPI_BOOST_PREDEF_DECLARE_TEST(SPI_BOOST_ARCH_X86_32,SPI_BOOST_ARCH_X86_32_NAME)
