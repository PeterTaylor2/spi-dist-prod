/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SPI_BOOST_PREDEF_LIBRARY_STD_MSL_H
#define SPI_BOOST_PREDEF_LIBRARY_STD_MSL_H

#include <spi_boost/predef/library/std/_prefix.h>

#include <spi_boost/predef/version_number.h>
#include <spi_boost/predef/make.h>

/*`
[heading `SPI_BOOST_LIB_STD_MSL`]

[@http://www.freescale.com/ Metrowerks] Standard C++ Library.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__MSL_CPP__`] [__predef_detection__]]
    [[`__MSL__`] [__predef_detection__]]

    [[`__MSL_CPP__`] [V.R.P]]
    [[`__MSL__`] [V.R.P]]
    ]
 */

#define SPI_BOOST_LIB_STD_MSL SPI_BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__MSL_CPP__) || defined(__MSL__)
#   undef SPI_BOOST_LIB_STD_MSL
#   if defined(__MSL_CPP__)
#       define SPI_BOOST_LIB_STD_MSL SPI_BOOST_PREDEF_MAKE_0X_VRPP(__MSL_CPP__)
#   else
#       define SPI_BOOST_LIB_STD_MSL SPI_BOOST_PREDEF_MAKE_0X_VRPP(__MSL__)
#   endif
#endif

#if SPI_BOOST_LIB_STD_MSL
#   define SPI_BOOST_LIB_STD_MSL_AVAILABLE
#endif

#define SPI_BOOST_LIB_STD_MSL_NAME "Metrowerks"

#endif

#include <spi_boost/predef/detail/test.h>
SPI_BOOST_PREDEF_DECLARE_TEST(SPI_BOOST_LIB_STD_MSL,SPI_BOOST_LIB_STD_MSL_NAME)
