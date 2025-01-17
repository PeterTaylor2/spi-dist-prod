//  (C) Copyright John Maddock 2001.
//  (C) Copyright Jens Maurer 2001.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  config for libstdc++ v3
//  not much to go in here:

#define SPI_BOOST_GNU_STDLIB 1

#ifdef __GLIBCXX__
#define SPI_BOOST_STDLIB "GNU libstdc++ version " SPI_BOOST_STRINGIZE(__GLIBCXX__)
#else
#define SPI_BOOST_STDLIB "GNU libstdc++ version " SPI_BOOST_STRINGIZE(__GLIBCPP__)
#endif

#if !defined(_GLIBCPP_USE_WCHAR_T) && !defined(_GLIBCXX_USE_WCHAR_T)
#  define SPI_BOOST_NO_CWCHAR
#  define SPI_BOOST_NO_CWCTYPE
#  define SPI_BOOST_NO_STD_WSTRING
#  define SPI_BOOST_NO_STD_WSTREAMBUF
#endif

#if defined(__osf__) && !defined(_REENTRANT) \
  && ( defined(_GLIBCXX_HAVE_GTHR_DEFAULT) || defined(_GLIBCPP_HAVE_GTHR_DEFAULT) )
// GCC 3 on Tru64 forces the definition of _REENTRANT when any std lib header
// file is included, therefore for consistency we define it here as well.
#  define _REENTRANT
#endif

#ifdef __GLIBCXX__ // gcc 3.4 and greater:
#  if defined(_GLIBCXX_HAVE_GTHR_DEFAULT) \
        || defined(_GLIBCXX__PTHREADS) \
        || defined(_GLIBCXX_HAS_GTHREADS) \
        || defined(_WIN32) \
        || defined(_AIX) \
        || defined(__HAIKU__)
      //
      // If the std lib has thread support turned on, then turn it on in Boost
      // as well.  We do this because some gcc-3.4 std lib headers define _REENTANT
      // while others do not...
      //
#     define SPI_BOOST_HAS_THREADS
#  else
#     define SPI_BOOST_DISABLE_THREADS
#  endif
#elif defined(__GLIBCPP__) \
        && !defined(_GLIBCPP_HAVE_GTHR_DEFAULT) \
        && !defined(_GLIBCPP__PTHREADS)
   // disable thread support if the std lib was built single threaded:
#  define SPI_BOOST_DISABLE_THREADS
#endif

#if (defined(linux) || defined(__linux) || defined(__linux__)) && defined(__arm__) && defined(_GLIBCPP_HAVE_GTHR_DEFAULT)
// linux on arm apparently doesn't define _REENTRANT
// so just turn on threading support whenever the std lib is thread safe:
#  define SPI_BOOST_HAS_THREADS
#endif

#if !defined(_GLIBCPP_USE_LONG_LONG) \
    && !defined(_GLIBCXX_USE_LONG_LONG)\
    && defined(SPI_BOOST_HAS_LONG_LONG)
// May have been set by compiler/*.hpp, but "long long" without library
// support is useless.
#  undef SPI_BOOST_HAS_LONG_LONG
#endif

// Apple doesn't seem to reliably defined a *unix* macro
#if !defined(CYGWIN) && (  defined(__unix__)  \
                        || defined(__unix)    \
                        || defined(unix)      \
                        || defined(__APPLE__) \
                        || defined(__APPLE)   \
                        || defined(APPLE))
#  include <unistd.h>
#endif

#ifndef __VXWORKS__ // VxWorks uses Dinkum, not GNU STL with GCC 
#if defined(__GLIBCXX__) || (defined(__GLIBCPP__) && __GLIBCPP__>=20020514) // GCC >= 3.1.0
#  define SPI_BOOST_STD_EXTENSION_NAMESPACE __gnu_cxx
#  define SPI_BOOST_HAS_SLIST
#  define SPI_BOOST_HAS_HASH
#  define SPI_BOOST_SLIST_HEADER <ext/slist>
# if !defined(__GNUC__) || __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 3)
#   define SPI_BOOST_HASH_SET_HEADER <ext/hash_set>
#   define SPI_BOOST_HASH_MAP_HEADER <ext/hash_map>
# else
#   define SPI_BOOST_HASH_SET_HEADER <backward/hash_set>
#   define SPI_BOOST_HASH_MAP_HEADER <backward/hash_map>
# endif
#endif
#endif

//
// Decide whether we have C++11 support turned on:
//
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103)
#  define SPI_BOOST_LIBSTDCXX11
#endif

//
//  Decide which version of libstdc++ we have, normally
//  libstdc++ C++0x support is detected via __GNUC__, __GNUC_MINOR__, and possibly
//  __GNUC_PATCHLEVEL__ at the suggestion of Jonathan Wakely, one of the libstdc++
//  developers. He also commented:
//
//       "I'm not sure how useful __GLIBCXX__ is for your purposes, for instance in
//       GCC 4.2.4 it is set to 20080519 but in GCC 4.3.0 it is set to 20080305.
//       Although 4.3.0 was released earlier than 4.2.4, it has better C++0x support
//       than any release in the 4.2 series."
//
//  Another resource for understanding libstdc++ features is:
//  http://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#manual.intro.status.standard.200x
//
//  However, using the GCC version number fails when the compiler is clang since this
//  only ever claims to emulate GCC-4.2, see https://svn.spi_boost.org/trac/spi_boost/ticket/7473
//  for a long discussion on this issue.  What we can do though is use clang's __has_include
//  to detect the presence of a C++11 header that was introduced with a specific GCC release.
//  We still have to be careful though as many such headers were buggy and/or incomplete when
//  first introduced, so we only check for headers that were fully featured from day 1, and then
//  use that to infer the underlying GCC version:
//
#ifdef __clang__

#if __has_include(<experimental/memory_resource>)
#  define SPI_BOOST_LIBSTDCXX_VERSION 60100
#elif __has_include(<experimental/any>)
#  define SPI_BOOST_LIBSTDCXX_VERSION 50100
#elif __has_include(<shared_mutex>)
#  define SPI_BOOST_LIBSTDCXX_VERSION 40900
#elif __has_include(<ext/cmath>)
#  define SPI_BOOST_LIBSTDCXX_VERSION 40800
#elif __has_include(<scoped_allocator>)
#  define SPI_BOOST_LIBSTDCXX_VERSION 40700
#elif __has_include(<typeindex>)
#  define SPI_BOOST_LIBSTDCXX_VERSION 40600
#elif __has_include(<future>)
#  define SPI_BOOST_LIBSTDCXX_VERSION 40500
#elif  __has_include(<ratio>)
#  define SPI_BOOST_LIBSTDCXX_VERSION 40400
#elif __has_include(<array>)
#  define SPI_BOOST_LIBSTDCXX_VERSION 40300
#endif

#if (SPI_BOOST_LIBSTDCXX_VERSION < 50100)
// libstdc++ does not define this function as it's deprecated in C++11, but clang still looks for it,
// defining it here is a terrible cludge, but should get things working:
extern "C" char *gets (char *__s);
#endif
//
// clang is unable to parse some GCC headers, add those workarounds here:
//
#if SPI_BOOST_LIBSTDCXX_VERSION < 50000
#  define SPI_BOOST_NO_CXX11_HDR_REGEX
#endif
//
// GCC 4.7.x has no __cxa_thread_atexit which
// thread_local objects require for cleanup:
//
#if SPI_BOOST_LIBSTDCXX_VERSION < 40800
#  define SPI_BOOST_NO_CXX11_THREAD_LOCAL
#endif
//
// Early clang versions can handle <chrono>, not exactly sure which versions
// but certainly up to clang-3.8 and gcc-4.6:
//
#if (__clang_major__ < 5)
#  if SPI_BOOST_LIBSTDCXX_VERSION < 40800
#     define SPI_BOOST_NO_CXX11_HDR_FUTURE
#     define SPI_BOOST_NO_CXX11_HDR_MUTEX
#     define SPI_BOOST_NO_CXX11_HDR_CONDITION_VARIABLE
#     define SPI_BOOST_NO_CXX11_HDR_CHRONO
#  endif
#endif

//
//  GCC 4.8 and 9 add working versions of <atomic> and <regex> respectively.
//  However, we have no test for these as the headers were present but broken
//  in early GCC versions.
//
#endif

#if defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x5130) && (__cplusplus >= 201103L)
//
// Oracle Solaris compiler uses it's own verison of libstdc++ but doesn't 
// set __GNUC__
//
#if __SUNPRO_CC >= 0x5140
#define SPI_BOOST_LIBSTDCXX_VERSION 50100
#else
#define SPI_BOOST_LIBSTDCXX_VERSION 40800
#endif
#endif

#if !defined(SPI_BOOST_LIBSTDCXX_VERSION)
#  define SPI_BOOST_LIBSTDCXX_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

// std::auto_ptr isn't provided with _GLIBCXX_DEPRECATED=0 (GCC 4.5 and earlier)
// or _GLIBCXX_USE_DEPRECATED=0 (GCC 4.6 and later).
#if defined(SPI_BOOST_LIBSTDCXX11)
#  if SPI_BOOST_LIBSTDCXX_VERSION < 40600
#     if !_GLIBCXX_DEPRECATED
#        define SPI_BOOST_NO_AUTO_PTR
#     endif
#  elif !_GLIBCXX_USE_DEPRECATED
#     define SPI_BOOST_NO_AUTO_PTR
#  endif
#endif

//  C++0x headers in GCC 4.3.0 and later
//
#if (SPI_BOOST_LIBSTDCXX_VERSION < 40300) || !defined(SPI_BOOST_LIBSTDCXX11)
#  define SPI_BOOST_NO_CXX11_HDR_ARRAY
#  define SPI_BOOST_NO_CXX11_HDR_TUPLE
#  define SPI_BOOST_NO_CXX11_HDR_UNORDERED_MAP
#  define SPI_BOOST_NO_CXX11_HDR_UNORDERED_SET
#  define SPI_BOOST_NO_CXX11_HDR_FUNCTIONAL
#endif

//  C++0x headers in GCC 4.4.0 and later
//
#if (SPI_BOOST_LIBSTDCXX_VERSION < 40400) || !defined(SPI_BOOST_LIBSTDCXX11)
#  define SPI_BOOST_NO_CXX11_HDR_CONDITION_VARIABLE
#  define SPI_BOOST_NO_CXX11_HDR_FORWARD_LIST
#  define SPI_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
#  define SPI_BOOST_NO_CXX11_HDR_MUTEX
#  define SPI_BOOST_NO_CXX11_HDR_RATIO
#  define SPI_BOOST_NO_CXX11_HDR_SYSTEM_ERROR
#  define SPI_BOOST_NO_CXX11_SMART_PTR
#else
#  define SPI_BOOST_HAS_TR1_COMPLEX_INVERSE_TRIG 
#  define SPI_BOOST_HAS_TR1_COMPLEX_OVERLOADS 
#endif

//  C++0x features in GCC 4.5.0 and later
//
#if (SPI_BOOST_LIBSTDCXX_VERSION < 40500) || !defined(SPI_BOOST_LIBSTDCXX11)
#  define SPI_BOOST_NO_CXX11_NUMERIC_LIMITS
#  define SPI_BOOST_NO_CXX11_HDR_FUTURE
#  define SPI_BOOST_NO_CXX11_HDR_RANDOM
#endif

//  C++0x features in GCC 4.6.0 and later
//
#if (SPI_BOOST_LIBSTDCXX_VERSION < 40600) || !defined(SPI_BOOST_LIBSTDCXX11)
#  define SPI_BOOST_NO_CXX11_HDR_TYPEINDEX
#  define SPI_BOOST_NO_CXX11_ADDRESSOF
#  define SPI_BOOST_NO_CXX17_ITERATOR_TRAITS
#endif

//  C++0x features in GCC 4.7.0 and later
//
#if (SPI_BOOST_LIBSTDCXX_VERSION < 40700) || !defined(SPI_BOOST_LIBSTDCXX11)
// Note that although <chrono> existed prior to 4.7, "steady_clock" is spelled "monotonic_clock"
// so 4.7.0 is the first truly conforming one.
#  define SPI_BOOST_NO_CXX11_HDR_CHRONO
#  define SPI_BOOST_NO_CXX11_ALLOCATOR
#  define SPI_BOOST_NO_CXX11_POINTER_TRAITS
#endif
//  C++0x features in GCC 4.8.0 and later
//
#if (SPI_BOOST_LIBSTDCXX_VERSION < 40800) || !defined(SPI_BOOST_LIBSTDCXX11)
// Note that although <atomic> existed prior to gcc 4.8 it was largely unimplemented for many types:
#  define SPI_BOOST_NO_CXX11_HDR_ATOMIC
#  define SPI_BOOST_NO_CXX11_HDR_THREAD
#endif
//  C++0x features in GCC 4.9.0 and later
//
#if (SPI_BOOST_LIBSTDCXX_VERSION < 40900) || !defined(SPI_BOOST_LIBSTDCXX11)
// Although <regex> is present and compilable against, the actual implementation is not functional
// even for the simplest patterns such as "\d" or "[0-9]". This is the case at least in gcc up to 4.8, inclusively.
#  define SPI_BOOST_NO_CXX11_HDR_REGEX
#endif
#if (SPI_BOOST_LIBSTDCXX_VERSION < 40900) || (__cplusplus <= 201103)
#  define SPI_BOOST_NO_CXX14_STD_EXCHANGE
#endif

#if defined(__clang_major__) && ((__clang_major__ < 3) || ((__clang_major__ == 3) && (__clang_minor__ < 7)))
// As of clang-3.6, libstdc++ header <atomic> throws up errors with clang:
#  define SPI_BOOST_NO_CXX11_HDR_ATOMIC
#endif
//
//  C++0x features in GCC 5.1 and later
//
#if (SPI_BOOST_LIBSTDCXX_VERSION < 50100) || !defined(SPI_BOOST_LIBSTDCXX11)
#  define SPI_BOOST_NO_CXX11_HDR_TYPE_TRAITS
#  define SPI_BOOST_NO_CXX11_HDR_CODECVT
#  define SPI_BOOST_NO_CXX11_ATOMIC_SMART_PTR
#  define SPI_BOOST_NO_CXX11_STD_ALIGN
#endif

//
//  C++17 features in GCC 7.1 and later
//
#if (SPI_BOOST_LIBSTDCXX_VERSION < 70100) || (__cplusplus <= 201402L)
#  define SPI_BOOST_NO_CXX17_STD_INVOKE
#  define SPI_BOOST_NO_CXX17_STD_APPLY
#  define SPI_BOOST_NO_CXX17_HDR_OPTIONAL
#  define SPI_BOOST_NO_CXX17_HDR_STRING_VIEW
#endif

#if defined(__has_include)
#if !__has_include(<shared_mutex>)
#  define SPI_BOOST_NO_CXX14_HDR_SHARED_MUTEX
#elif __cplusplus <= 201103
#  define SPI_BOOST_NO_CXX14_HDR_SHARED_MUTEX
#endif
#elif __cplusplus < 201402 || (SPI_BOOST_LIBSTDCXX_VERSION < 40900) || !defined(SPI_BOOST_LIBSTDCXX11)
#  define SPI_BOOST_NO_CXX14_HDR_SHARED_MUTEX
#endif

//
// Headers not present on Solaris with the Oracle compiler:
#if defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5140)
#define SPI_BOOST_NO_CXX11_HDR_FUTURE
#define SPI_BOOST_NO_CXX11_HDR_FORWARD_LIST 
#define SPI_BOOST_NO_CXX11_HDR_ATOMIC
// shared_ptr is present, but is not convertible to bool
// which causes all kinds of problems especially in Boost.Thread
// but probably elsewhere as well.
#define SPI_BOOST_NO_CXX11_SMART_PTR
#endif

#if (!defined(_GLIBCXX_HAS_GTHREADS) || !defined(_GLIBCXX_USE_C99_STDINT_TR1))
   // Headers not always available:
#  ifndef SPI_BOOST_NO_CXX11_HDR_CONDITION_VARIABLE
#     define SPI_BOOST_NO_CXX11_HDR_CONDITION_VARIABLE
#  endif
#  ifndef SPI_BOOST_NO_CXX11_HDR_MUTEX
#     define SPI_BOOST_NO_CXX11_HDR_MUTEX
#  endif
#  ifndef SPI_BOOST_NO_CXX11_HDR_THREAD
#     define SPI_BOOST_NO_CXX11_HDR_THREAD
#  endif
#  ifndef SPI_BOOST_NO_CXX14_HDR_SHARED_MUTEX
#     define SPI_BOOST_NO_CXX14_HDR_SHARED_MUTEX
#  endif
#endif

#if (!defined(_GTHREAD_USE_MUTEX_TIMEDLOCK) || (_GTHREAD_USE_MUTEX_TIMEDLOCK == 0)) && !defined(SPI_BOOST_NO_CXX11_HDR_MUTEX)
// Timed mutexes are not always available:
#  define SPI_BOOST_NO_CXX11_HDR_MUTEX
#endif

//  --- end ---
