#ifndef SPI_BOOST_SMART_PTR_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED
#define SPI_BOOST_SMART_PTR_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  detail/sp_counted_impl.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <spi_boost/config.hpp>

#if defined(SPI_BOOST_SP_USE_STD_ALLOCATOR) && defined(SPI_BOOST_SP_USE_QUICK_ALLOCATOR)
# error SPI_BOOST_SP_USE_STD_ALLOCATOR and SPI_BOOST_SP_USE_QUICK_ALLOCATOR are incompatible.
#endif

#include <spi_boost/checked_delete.hpp>
#include <spi_boost/smart_ptr/detail/sp_counted_base.hpp>
#include <spi_boost/smart_ptr/detail/sp_noexcept.hpp>
#include <spi_boost/core/addressof.hpp>

#if defined(SPI_BOOST_SP_USE_QUICK_ALLOCATOR)
#include <spi_boost/smart_ptr/detail/quick_allocator.hpp>
#endif

#if defined(SPI_BOOST_SP_USE_STD_ALLOCATOR)
#include <memory>           // std::allocator
#endif

#include <cstddef>          // std::size_t

namespace spi_boost
{

#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)

void sp_scalar_constructor_hook( void * px, std::size_t size, void * pn );
void sp_scalar_destructor_hook( void * px, std::size_t size, void * pn );

#endif

namespace detail
{

// get_local_deleter

template<class D> class local_sp_deleter;

template<class D> D * get_local_deleter( D * /*p*/ ) SPI_BOOST_SP_NOEXCEPT
{
    return 0;
}

template<class D> D * get_local_deleter( local_sp_deleter<D> * p ) SPI_BOOST_SP_NOEXCEPT;

//

template<class X> class SPI_BOOST_SYMBOL_VISIBLE sp_counted_impl_p: public sp_counted_base
{
private:

    X * px_;

    sp_counted_impl_p( sp_counted_impl_p const & );
    sp_counted_impl_p & operator= ( sp_counted_impl_p const & );

    typedef sp_counted_impl_p<X> this_type;

public:

    explicit sp_counted_impl_p( X * px ): px_( px )
    {
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        spi_boost::sp_scalar_constructor_hook( px, sizeof(X), this );
#endif
    }

    virtual void dispose() SPI_BOOST_SP_NOEXCEPT
    {
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        spi_boost::sp_scalar_destructor_hook( px_, sizeof(X), this );
#endif
        spi_boost::checked_delete( px_ );
    }

    virtual void * get_deleter( sp_typeinfo const & ) SPI_BOOST_SP_NOEXCEPT
    {
        return 0;
    }

    virtual void * get_local_deleter( sp_typeinfo const & ) SPI_BOOST_SP_NOEXCEPT
    {
        return 0;
    }

    virtual void * get_untyped_deleter() SPI_BOOST_SP_NOEXCEPT
    {
        return 0;
    }

#if defined(SPI_BOOST_SP_USE_STD_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
    }

    void operator delete( void * p )
    {
        std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }

#endif

#if defined(SPI_BOOST_SP_USE_QUICK_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return quick_allocator<this_type>::alloc();
    }

    void operator delete( void * p )
    {
        quick_allocator<this_type>::dealloc( p );
    }

#endif
};

//
// Borland's Codeguard trips up over the -Vx- option here:
//
#ifdef __CODEGUARD__
# pragma option push -Vx-
#endif

template<class P, class D> class SPI_BOOST_SYMBOL_VISIBLE sp_counted_impl_pd: public sp_counted_base
{
private:

    P ptr; // copy constructor must not throw
    D del; // copy constructor must not throw

    sp_counted_impl_pd( sp_counted_impl_pd const & );
    sp_counted_impl_pd & operator= ( sp_counted_impl_pd const & );

    typedef sp_counted_impl_pd<P, D> this_type;

public:

    // pre: d(p) must not throw

    sp_counted_impl_pd( P p, D & d ): ptr( p ), del( d )
    {
    }

    sp_counted_impl_pd( P p ): ptr( p ), del()
    {
    }

    virtual void dispose() SPI_BOOST_SP_NOEXCEPT
    {
        del( ptr );
    }

    virtual void * get_deleter( sp_typeinfo const & ti ) SPI_BOOST_SP_NOEXCEPT
    {
        return ti == SPI_BOOST_SP_TYPEID(D)? &reinterpret_cast<char&>( del ): 0;
    }

    virtual void * get_local_deleter( sp_typeinfo const & ti ) SPI_BOOST_SP_NOEXCEPT
    {
        return ti == SPI_BOOST_SP_TYPEID(D)? spi_boost::detail::get_local_deleter( spi_boost::addressof( del ) ): 0;
    }

    virtual void * get_untyped_deleter() SPI_BOOST_SP_NOEXCEPT
    {
        return &reinterpret_cast<char&>( del );
    }

#if defined(SPI_BOOST_SP_USE_STD_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
    }

    void operator delete( void * p )
    {
        std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }

#endif

#if defined(SPI_BOOST_SP_USE_QUICK_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return quick_allocator<this_type>::alloc();
    }

    void operator delete( void * p )
    {
        quick_allocator<this_type>::dealloc( p );
    }

#endif
};

template<class P, class D, class A> class SPI_BOOST_SYMBOL_VISIBLE sp_counted_impl_pda: public sp_counted_base
{
private:

    P p_; // copy constructor must not throw
    D d_; // copy constructor must not throw
    A a_; // copy constructor must not throw

    sp_counted_impl_pda( sp_counted_impl_pda const & );
    sp_counted_impl_pda & operator= ( sp_counted_impl_pda const & );

    typedef sp_counted_impl_pda<P, D, A> this_type;

public:

    // pre: d( p ) must not throw

    sp_counted_impl_pda( P p, D & d, A a ): p_( p ), d_( d ), a_( a )
    {
    }

    sp_counted_impl_pda( P p, A a ): p_( p ), d_( a ), a_( a )
    {
    }

    virtual void dispose() SPI_BOOST_SP_NOEXCEPT
    {
        d_( p_ );
    }

    virtual void destroy() SPI_BOOST_SP_NOEXCEPT
    {
#if !defined( SPI_BOOST_NO_CXX11_ALLOCATOR )

        typedef typename std::allocator_traits<A>::template rebind_alloc< this_type > A2;

#else

        typedef typename A::template rebind< this_type >::other A2;

#endif

        A2 a2( a_ );

        this->~this_type();

        a2.deallocate( this, 1 );
    }

    virtual void * get_deleter( sp_typeinfo const & ti ) SPI_BOOST_SP_NOEXCEPT
    {
        return ti == SPI_BOOST_SP_TYPEID( D )? &reinterpret_cast<char&>( d_ ): 0;
    }

    virtual void * get_local_deleter( sp_typeinfo const & ti ) SPI_BOOST_SP_NOEXCEPT
    {
        return ti == SPI_BOOST_SP_TYPEID(D)? spi_boost::detail::get_local_deleter( spi_boost::addressof( d_ ) ): 0;
    }

    virtual void * get_untyped_deleter() SPI_BOOST_SP_NOEXCEPT
    {
        return &reinterpret_cast<char&>( d_ );
    }
};

#ifdef __CODEGUARD__
# pragma option pop
#endif

} // namespace detail

} // namespace spi_boost

#endif  // #ifndef SPI_BOOST_SMART_PTR_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED
