/*
Copyright (c) Microsoft Corporation 2014
Copyright Rene Rivera 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SPI_BOOST_PREDEF_PLAT_WINDOWS_PHONE_H
#define SPI_BOOST_PREDEF_PLAT_WINDOWS_PHONE_H

#include <spi_boost/predef/make.h>
#include <spi_boost/predef/os/windows.h>
#include <spi_boost/predef/platform/windows_uwp.h>
#include <spi_boost/predef/version_number.h>

/*`
[heading `SPI_BOOST_PLAT_WINDOWS_PHONE`]

[@https://docs.microsoft.com/en-us/windows/uwp/get-started/universal-application-platform-guide UWP]
for Windows Phone development.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP`] [__predef_detection__]]
    ]
 */

#define SPI_BOOST_PLAT_WINDOWS_PHONE SPI_BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if SPI_BOOST_OS_WINDOWS && \
    defined(WINAPI_FAMILY_PHONE_APP) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#   undef SPI_BOOST_PLAT_WINDOWS_PHONE
#   define SPI_BOOST_PLAT_WINDOWS_PHONE SPI_BOOST_VERSION_NUMBER_AVAILABLE
#endif
 
#if SPI_BOOST_PLAT_WINDOWS_PHONE
#   define SPI_BOOST_PLAT_WINDOWS_PHONE_AVAILABLE
#   include <spi_boost/predef/detail/platform_detected.h>
#endif

#define SPI_BOOST_PLAT_WINDOWS_PHONE_NAME "Windows Phone"

#endif

#include <spi_boost/predef/detail/test.h>
SPI_BOOST_PREDEF_DECLARE_TEST(SPI_BOOST_PLAT_WINDOWS_PHONE,SPI_BOOST_PLAT_WINDOWS_PHONE_NAME)
