#ifndef SPI_BOOST_CURRENT_FUNCTION_HPP_INCLUDED
#define SPI_BOOST_CURRENT_FUNCTION_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  spi_boost/current_function.hpp - SPI_BOOST_CURRENT_FUNCTION
//
//  Copyright 2002-2018 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  http://www.boost.org/libs/assert
//

namespace spi_boost
{

namespace detail
{

inline void current_function_helper()
{

#if defined( SPI_BOOST_DISABLE_CURRENT_FUNCTION )

# define SPI_BOOST_CURRENT_FUNCTION "(unknown)"

#elif defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__) || defined(__clang__)

# define SPI_BOOST_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__DMC__) && (__DMC__ >= 0x810)

# define SPI_BOOST_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__FUNCSIG__)

# define SPI_BOOST_CURRENT_FUNCTION __FUNCSIG__

#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))

# define SPI_BOOST_CURRENT_FUNCTION __FUNCTION__

#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)

# define SPI_BOOST_CURRENT_FUNCTION __FUNC__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)

# define SPI_BOOST_CURRENT_FUNCTION __func__

#elif defined(__cplusplus) && (__cplusplus >= 201103)

# define SPI_BOOST_CURRENT_FUNCTION __func__

#else

# define SPI_BOOST_CURRENT_FUNCTION "(unknown)"

#endif

}

} // namespace detail

} // namespace spi_boost

#endif // #ifndef SPI_BOOST_CURRENT_FUNCTION_HPP_INCLUDED
