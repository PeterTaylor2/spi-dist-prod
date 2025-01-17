/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SPI_BOOST_PREDEF_COMPILER_MPW_H
#define SPI_BOOST_PREDEF_COMPILER_MPW_H

#include <spi_boost/predef/version_number.h>
#include <spi_boost/predef/make.h>

/*`
[heading `SPI_BOOST_COMP_MPW`]

[@http://en.wikipedia.org/wiki/Macintosh_Programmer%27s_Workshop MPW C++] compiler.
Version number available as major, and minor.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__MRC__`] [__predef_detection__]]
    [[`MPW_C`] [__predef_detection__]]
    [[`MPW_CPLUS`] [__predef_detection__]]

    [[`__MRC__`] [V.R.0]]
    ]
 */

#define SPI_BOOST_COMP_MPW SPI_BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__MRC__) || defined(MPW_C) || defined(MPW_CPLUS)
#   if !defined(SPI_BOOST_COMP_MPW_DETECTION) && defined(__MRC__)
#       define SPI_BOOST_COMP_MPW_DETECTION SPI_BOOST_PREDEF_MAKE_0X_VVRR(__MRC__)
#   endif
#   if !defined(SPI_BOOST_COMP_MPW_DETECTION)
#       define SPI_BOOST_COMP_MPW_DETECTION SPI_BOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#ifdef SPI_BOOST_COMP_MPW_DETECTION
#   if defined(SPI_BOOST_PREDEF_DETAIL_COMP_DETECTED)
#       define SPI_BOOST_COMP_MPW_EMULATED SPI_BOOST_COMP_MPW_DETECTION
#   else
#       undef SPI_BOOST_COMP_MPW
#       define SPI_BOOST_COMP_MPW SPI_BOOST_COMP_MPW_DETECTION
#   endif
#   define SPI_BOOST_COMP_MPW_AVAILABLE
#   include <spi_boost/predef/detail/comp_detected.h>
#endif

#define SPI_BOOST_COMP_MPW_NAME "MPW C++"

#endif

#include <spi_boost/predef/detail/test.h>
SPI_BOOST_PREDEF_DECLARE_TEST(SPI_BOOST_COMP_MPW,SPI_BOOST_COMP_MPW_NAME)

#ifdef SPI_BOOST_COMP_MPW_EMULATED
#include <spi_boost/predef/detail/test.h>
SPI_BOOST_PREDEF_DECLARE_TEST(SPI_BOOST_COMP_MPW_EMULATED,SPI_BOOST_COMP_MPW_NAME)
#endif
