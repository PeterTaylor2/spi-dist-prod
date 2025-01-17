//  Copyright (c) 2017 Dynatrace
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for most recent version.

//  Standard library setup for IBM z/OS XL C/C++ compiler.

// Oldest library version currently supported is 2.1 (V2R1)
#if __TARGET_LIB__ < 0x42010000
#  error "Library version not supported or configured - please reconfigure"
#endif

#if __TARGET_LIB__ > 0x42010000
#  if defined(SPI_BOOST_ASSERT_CONFIG)
#     error "Unknown library version - please run the configure tests and report the results"
#  endif
#endif

#define SPI_BOOST_STDLIB "IBM z/OS XL C/C++ standard library"

#define SPI_BOOST_HAS_MACRO_USE_FACET

#define SPI_BOOST_NO_CXX11_HDR_TYPE_TRAITS
#define SPI_BOOST_NO_CXX11_HDR_INITIALIZER_LIST

#define SPI_BOOST_NO_CXX11_ADDRESSOF
#define SPI_BOOST_NO_CXX11_SMART_PTR
#define SPI_BOOST_NO_CXX11_ATOMIC_SMART_PTR
#define SPI_BOOST_NO_CXX11_NUMERIC_LIMITS
#define SPI_BOOST_NO_CXX11_ALLOCATOR
#define SPI_BOOST_NO_CXX11_POINTER_TRAITS
#define SPI_BOOST_NO_CXX11_HDR_FUNCTIONAL
#define SPI_BOOST_NO_CXX11_HDR_UNORDERED_SET
#define SPI_BOOST_NO_CXX11_HDR_UNORDERED_MAP
#define SPI_BOOST_NO_CXX11_HDR_TYPEINDEX
#define SPI_BOOST_NO_CXX11_HDR_TUPLE
#define SPI_BOOST_NO_CXX11_HDR_THREAD
#define SPI_BOOST_NO_CXX11_HDR_SYSTEM_ERROR
#define SPI_BOOST_NO_CXX11_HDR_REGEX
#define SPI_BOOST_NO_CXX11_HDR_RATIO
#define SPI_BOOST_NO_CXX11_HDR_RANDOM
#define SPI_BOOST_NO_CXX11_HDR_MUTEX
#define SPI_BOOST_NO_CXX11_HDR_FUTURE
#define SPI_BOOST_NO_CXX11_HDR_FORWARD_LIST
#define SPI_BOOST_NO_CXX11_HDR_CONDITION_VARIABLE
#define SPI_BOOST_NO_CXX11_HDR_CODECVT
#define SPI_BOOST_NO_CXX11_HDR_CHRONO
#define SPI_BOOST_NO_CXX11_HDR_ATOMIC
#define SPI_BOOST_NO_CXX11_HDR_ARRAY
#define SPI_BOOST_NO_CXX11_STD_ALIGN

#define SPI_BOOST_NO_CXX14_STD_EXCHANGE
#define SPI_BOOST_NO_CXX14_HDR_SHARED_MUTEX

#define SPI_BOOST_NO_CXX17_STD_INVOKE
#define SPI_BOOST_NO_CXX17_STD_APPLY
#define SPI_BOOST_NO_CXX17_ITERATOR_TRAITS
