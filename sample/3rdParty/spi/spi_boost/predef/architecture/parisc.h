/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SPI_BOOST_PREDEF_ARCHITECTURE_PARISC_H
#define SPI_BOOST_PREDEF_ARCHITECTURE_PARISC_H

#include <spi_boost/predef/version_number.h>
#include <spi_boost/predef/make.h>

/*`
[heading `SPI_BOOST_ARCH_PARISC`]

[@http://en.wikipedia.org/wiki/PA-RISC_family HP/PA RISC] architecture.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__hppa__`] [__predef_detection__]]
    [[`__hppa`] [__predef_detection__]]
    [[`__HPPA__`] [__predef_detection__]]

    [[`_PA_RISC1_0`] [1.0.0]]
    [[`_PA_RISC1_1`] [1.1.0]]
    [[`__HPPA11__`] [1.1.0]]
    [[`__PA7100__`] [1.1.0]]
    [[`_PA_RISC2_0`] [2.0.0]]
    [[`__RISC2_0__`] [2.0.0]]
    [[`__HPPA20__`] [2.0.0]]
    [[`__PA8000__`] [2.0.0]]
    ]
 */

#define SPI_BOOST_ARCH_PARISC SPI_BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__hppa__) || defined(__hppa) || defined(__HPPA__)
#   undef SPI_BOOST_ARCH_PARISC
#   if !defined(SPI_BOOST_ARCH_PARISC) && (defined(_PA_RISC1_0))
#       define SPI_BOOST_ARCH_PARISC SPI_BOOST_VERSION_NUMBER(1,0,0)
#   endif
#   if !defined(SPI_BOOST_ARCH_PARISC) && (defined(_PA_RISC1_1) || defined(__HPPA11__) || defined(__PA7100__))
#       define SPI_BOOST_ARCH_PARISC SPI_BOOST_VERSION_NUMBER(1,1,0)
#   endif
#   if !defined(SPI_BOOST_ARCH_PARISC) && (defined(_PA_RISC2_0) || defined(__RISC2_0__) || defined(__HPPA20__) || defined(__PA8000__))
#       define SPI_BOOST_ARCH_PARISC SPI_BOOST_VERSION_NUMBER(2,0,0)
#   endif
#   if !defined(SPI_BOOST_ARCH_PARISC)
#       define SPI_BOOST_ARCH_PARISC SPI_BOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if SPI_BOOST_ARCH_PARISC
#   define SPI_BOOST_ARCH_PARISC_AVAILABLE
#endif

#define SPI_BOOST_ARCH_PARISC_NAME "HP/PA RISC"

#endif

#include <spi_boost/predef/detail/test.h>
SPI_BOOST_PREDEF_DECLARE_TEST(SPI_BOOST_ARCH_PARISC,SPI_BOOST_ARCH_PARISC_NAME)
