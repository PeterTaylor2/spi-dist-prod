/*
Copyright Benjamin Worpitz 2018
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SPI_BOOST_PREDEF_ARCHITECTURE_PTX_H
#define SPI_BOOST_PREDEF_ARCHITECTURE_PTX_H

#include <spi_boost/predef/version_number.h>
#include <spi_boost/predef/make.h>

/*`
[heading `SPI_BOOST_ARCH_PTX`]

[@https://en.wikipedia.org/wiki/Parallel_Thread_Execution PTX] architecture.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__CUDA_ARCH__`] [__predef_detection__]]

    [[`__CUDA_ARCH__`] [V.R.0]]
    ]
 */

#define SPI_BOOST_ARCH_PTX SPI_BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__CUDA_ARCH__)
#   undef SPI_BOOST_ARCH_PTX
#   define SPI_BOOST_ARCH_PTX SPI_BOOST_PREDEF_MAKE_10_VR0(__CUDA_ARCH__)
#endif

#if SPI_BOOST_ARCH_PTX
#   define SPI_BOOST_ARCH_PTX_AVAILABLE
#endif

#define SPI_BOOST_ARCH_PTX_NAME "PTX"

#endif

#include <spi_boost/predef/detail/test.h>
SPI_BOOST_PREDEF_DECLARE_TEST(SPI_BOOST_ARCH_PTX,SPI_BOOST_ARCH_PTX_NAME)
