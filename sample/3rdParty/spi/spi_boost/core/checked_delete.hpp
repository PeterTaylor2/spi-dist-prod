#ifndef SPI_BOOST_CORE_CHECKED_DELETE_HPP
#define SPI_BOOST_CORE_CHECKED_DELETE_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <spi_boost/config.hpp>

//
//  spi_boost/checked_delete.hpp
//
//  Copyright (c) 2002, 2003 Peter Dimov
//  Copyright (c) 2003 Daniel Frey
//  Copyright (c) 2003 Howard Hinnant
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/core/doc/html/core/checked_delete.html for documentation.
//

namespace spi_boost
{

// verify that types are complete for increased safety

template<class T> inline void checked_delete(T * x) SPI_BOOST_NOEXCEPT
{
    // intentionally complex - simplification causes regressions
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete x;
}

template<class T> inline void checked_array_delete(T * x) SPI_BOOST_NOEXCEPT
{
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete [] x;
}

template<class T> struct checked_deleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const SPI_BOOST_NOEXCEPT
    {
        // spi_boost:: disables ADL
        spi_boost::checked_delete(x);
    }
};

template<class T> struct checked_array_deleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const SPI_BOOST_NOEXCEPT
    {
        spi_boost::checked_array_delete(x);
    }
};

} // namespace spi_boost

#endif  // #ifndef SPI_BOOST_CORE_CHECKED_DELETE_HPP
