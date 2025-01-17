#ifndef SPI_BOOST_SMART_PTR_DETAIL_LWM_NOP_HPP_INCLUDED
#define SPI_BOOST_SMART_PTR_DETAIL_LWM_NOP_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  spi_boost/detail/lwm_nop.hpp
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

namespace spi_boost
{

namespace detail
{

class lightweight_mutex
{
public:

    typedef lightweight_mutex scoped_lock;
};

} // namespace detail

} // namespace spi_boost

#endif // #ifndef SPI_BOOST_SMART_PTR_DETAIL_LWM_NOP_HPP_INCLUDED
