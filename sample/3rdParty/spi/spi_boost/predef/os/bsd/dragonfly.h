/*
Copyright Rene Rivera 2012-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SPI_BOOST_PREDEF_OS_BSD_DRAGONFLY_H
#define SPI_BOOST_PREDEF_OS_BSD_DRAGONFLY_H

#include <spi_boost/predef/os/bsd.h>

/*`
[heading `SPI_BOOST_OS_BSD_DRAGONFLY`]

[@http://en.wikipedia.org/wiki/DragonFly_BSD DragonFly BSD] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__DragonFly__`] [__predef_detection__]]
    ]
 */

#define SPI_BOOST_OS_BSD_DRAGONFLY SPI_BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !defined(SPI_BOOST_PREDEF_DETAIL_OS_DETECTED) && ( \
    defined(__DragonFly__) \
    )
#   ifndef SPI_BOOST_OS_BSD_AVAILABLE
#       define SPI_BOOST_OS_BSD SPI_BOOST_VERSION_NUMBER_AVAILABLE
#       define SPI_BOOST_OS_BSD_AVAILABLE
#   endif
#   undef SPI_BOOST_OS_BSD_DRAGONFLY
#   if defined(__DragonFly__)
#       define SPI_BOOST_OS_DRAGONFLY_BSD SPI_BOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if SPI_BOOST_OS_BSD_DRAGONFLY
#   define SPI_BOOST_OS_BSD_DRAGONFLY_AVAILABLE
#   include <spi_boost/predef/detail/os_detected.h>
#endif

#define SPI_BOOST_OS_BSD_DRAGONFLY_NAME "DragonFly BSD"

#endif

#include <spi_boost/predef/detail/test.h>
SPI_BOOST_PREDEF_DECLARE_TEST(SPI_BOOST_OS_BSD_DRAGONFLY,SPI_BOOST_OS_BSD_DRAGONFLY_NAME)
