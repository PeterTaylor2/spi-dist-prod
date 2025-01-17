/*
 * Copyright (C) 2017 James E. King III
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef SPI_BOOST_PREDEF_LIBRARY_C_CLOUDABI_H
#define SPI_BOOST_PREDEF_LIBRARY_C_CLOUDABI_H

#include <spi_boost/predef/version_number.h>
#include <spi_boost/predef/make.h>

#include <spi_boost/predef/library/c/_prefix.h>

#if defined(__CloudABI__)
#include <stddef.h>
#endif

/*`
[heading `SPI_BOOST_LIB_C_CLOUDABI`]

[@https://github.com/NuxiNL/cloudlibc cloudlibc] - CloudABI's standard C library.
Version number available as major, and minor.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__cloudlibc__`] [__predef_detection__]]

    [[`__cloudlibc_major__`, `__cloudlibc_minor__`] [V.R.0]]
    ]
 */

#define SPI_BOOST_LIB_C_CLOUDABI SPI_BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__cloudlibc__)
#   undef SPI_BOOST_LIB_C_CLOUDABI
#   define SPI_BOOST_LIB_C_CLOUDABI \
            SPI_BOOST_VERSION_NUMBER(__cloudlibc_major__,__cloudlibc_minor__,0)
#endif

#if SPI_BOOST_LIB_C_CLOUDABI
#   define SPI_BOOST_LIB_C_CLOUDABI_AVAILABLE
#endif

#define SPI_BOOST_LIB_C_CLOUDABI_NAME "cloudlibc"

#endif

#include <spi_boost/predef/detail/test.h>
SPI_BOOST_PREDEF_DECLARE_TEST(SPI_BOOST_LIB_C_CLOUDABI,SPI_BOOST_LIB_C_CLOUDABI_NAME)
