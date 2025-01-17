#ifndef SPI_BOOST_SMART_PTR_DETAIL_SHARED_COUNT_HPP_INCLUDED
#define SPI_BOOST_SMART_PTR_DETAIL_SHARED_COUNT_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  detail/shared_count.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifdef __BORLANDC__
# pragma warn -8027     // Functions containing try are not expanded inline
#endif

#include <spi_boost/config.hpp>
#include <spi_boost/checked_delete.hpp>
#include <spi_boost/throw_exception.hpp>
#include <spi_boost/smart_ptr/bad_weak_ptr.hpp>
#include <spi_boost/smart_ptr/detail/sp_counted_base.hpp>
#include <spi_boost/smart_ptr/detail/sp_counted_impl.hpp>
#include <spi_boost/smart_ptr/detail/sp_disable_deprecated.hpp>
#include <spi_boost/smart_ptr/detail/sp_noexcept.hpp>
#include <spi_boost/config/workaround.hpp>
// In order to avoid circular dependencies with Boost.TR1
// we make sure that our include of <memory> doesn't try to
// pull in the TR1 headers: that's why we use this header 
// rather than including <memory> directly:
#include <spi_boost/config/no_tr1/memory.hpp>  // std::auto_ptr
#include <functional>       // std::less

#ifdef SPI_BOOST_NO_EXCEPTIONS
# include <new>              // std::bad_alloc
#endif

#include <spi_boost/core/addressof.hpp>

#if defined( SPI_BOOST_SP_DISABLE_DEPRECATED )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

namespace spi_boost
{

namespace movelib
{

template< class T, class D > class unique_ptr;

} // namespace movelib

namespace detail
{

#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)

int const shared_count_id = 0x2C35F101;
int const   weak_count_id = 0x298C38A4;

#endif

struct sp_nothrow_tag {};

template< class D > struct sp_inplace_tag
{
};

template< class T > class sp_reference_wrapper
{ 
public:

    explicit sp_reference_wrapper( T & t): t_( spi_boost::addressof( t ) )
    {
    }

    template< class Y > void operator()( Y * p ) const
    {
        (*t_)( p );
    }

private:

    T * t_;
};

template< class D > struct sp_convert_reference
{
    typedef D type;
};

template< class D > struct sp_convert_reference< D& >
{
    typedef sp_reference_wrapper< D > type;
};

class weak_count;

class shared_count
{
private:

    sp_counted_base * pi_;

#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
    int id_;
#endif

    friend class weak_count;

public:

    SPI_BOOST_CONSTEXPR shared_count() SPI_BOOST_SP_NOEXCEPT: pi_(0)
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
    }

    SPI_BOOST_CONSTEXPR explicit shared_count( sp_counted_base * pi ) SPI_BOOST_SP_NOEXCEPT: pi_( pi )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
    }

    template<class Y> explicit shared_count( Y * p ): pi_( 0 )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
#ifndef SPI_BOOST_NO_EXCEPTIONS

        try
        {
            pi_ = new sp_counted_impl_p<Y>( p );
        }
        catch(...)
        {
            spi_boost::checked_delete( p );
            throw;
        }

#else

        pi_ = new sp_counted_impl_p<Y>( p );

        if( pi_ == 0 )
        {
            spi_boost::checked_delete( p );
            spi_boost::throw_exception( std::bad_alloc() );
        }

#endif
    }

#if defined( SPI_BOOST_MSVC ) && SPI_BOOST_WORKAROUND( SPI_BOOST_MSVC, <= 1200 )
    template<class Y, class D> shared_count( Y * p, D d ): pi_(0)
#else
    template<class P, class D> shared_count( P p, D d ): pi_(0)
#endif
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
#if defined( SPI_BOOST_MSVC ) && SPI_BOOST_WORKAROUND( SPI_BOOST_MSVC, <= 1200 )
        typedef Y* P;
#endif
#ifndef SPI_BOOST_NO_EXCEPTIONS

        try
        {
            pi_ = new sp_counted_impl_pd<P, D>(p, d);
        }
        catch(...)
        {
            d(p); // delete p
            throw;
        }

#else

        pi_ = new sp_counted_impl_pd<P, D>(p, d);

        if(pi_ == 0)
        {
            d(p); // delete p
            spi_boost::throw_exception(std::bad_alloc());
        }

#endif
    }

#if !defined( SPI_BOOST_NO_FUNCTION_TEMPLATE_ORDERING )

    template< class P, class D > shared_count( P p, sp_inplace_tag<D> ): pi_( 0 )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
#ifndef SPI_BOOST_NO_EXCEPTIONS

        try
        {
            pi_ = new sp_counted_impl_pd< P, D >( p );
        }
        catch( ... )
        {
            D::operator_fn( p ); // delete p
            throw;
        }

#else

        pi_ = new sp_counted_impl_pd< P, D >( p );

        if( pi_ == 0 )
        {
            D::operator_fn( p ); // delete p
            spi_boost::throw_exception( std::bad_alloc() );
        }

#endif // #ifndef SPI_BOOST_NO_EXCEPTIONS
    }

#endif // !defined( SPI_BOOST_NO_FUNCTION_TEMPLATE_ORDERING )

    template<class P, class D, class A> shared_count( P p, D d, A a ): pi_( 0 )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
        typedef sp_counted_impl_pda<P, D, A> impl_type;

#if !defined( SPI_BOOST_NO_CXX11_ALLOCATOR )

        typedef typename std::allocator_traits<A>::template rebind_alloc< impl_type > A2;

#else

        typedef typename A::template rebind< impl_type >::other A2;

#endif

        A2 a2( a );

#ifndef SPI_BOOST_NO_EXCEPTIONS

        try
        {
            pi_ = a2.allocate( 1 );
            ::new( static_cast< void* >( pi_ ) ) impl_type( p, d, a );
        }
        catch(...)
        {
            d( p );

            if( pi_ != 0 )
            {
                a2.deallocate( static_cast< impl_type* >( pi_ ), 1 );
            }

            throw;
        }

#else

        pi_ = a2.allocate( 1 );

        if( pi_ != 0 )
        {
            ::new( static_cast< void* >( pi_ ) ) impl_type( p, d, a );
        }
        else
        {
            d( p );
            spi_boost::throw_exception( std::bad_alloc() );
        }

#endif
    }

#if !defined( SPI_BOOST_NO_FUNCTION_TEMPLATE_ORDERING )

    template< class P, class D, class A > shared_count( P p, sp_inplace_tag< D >, A a ): pi_( 0 )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
        typedef sp_counted_impl_pda< P, D, A > impl_type;

#if !defined( SPI_BOOST_NO_CXX11_ALLOCATOR )

        typedef typename std::allocator_traits<A>::template rebind_alloc< impl_type > A2;

#else

        typedef typename A::template rebind< impl_type >::other A2;

#endif

        A2 a2( a );

#ifndef SPI_BOOST_NO_EXCEPTIONS

        try
        {
            pi_ = a2.allocate( 1 );
            ::new( static_cast< void* >( pi_ ) ) impl_type( p, a );
        }
        catch(...)
        {
            D::operator_fn( p );

            if( pi_ != 0 )
            {
                a2.deallocate( static_cast< impl_type* >( pi_ ), 1 );
            }

            throw;
        }

#else

        pi_ = a2.allocate( 1 );

        if( pi_ != 0 )
        {
            ::new( static_cast< void* >( pi_ ) ) impl_type( p, a );
        }
        else
        {
            D::operator_fn( p );
            spi_boost::throw_exception( std::bad_alloc() );
        }

#endif // #ifndef SPI_BOOST_NO_EXCEPTIONS
    }

#endif // !defined( SPI_BOOST_NO_FUNCTION_TEMPLATE_ORDERING )

#ifndef SPI_BOOST_NO_AUTO_PTR

    // auto_ptr<Y> is special cased to provide the strong guarantee

    template<class Y>
    explicit shared_count( std::auto_ptr<Y> & r ): pi_( new sp_counted_impl_p<Y>( r.get() ) )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
#ifdef SPI_BOOST_NO_EXCEPTIONS

        if( pi_ == 0 )
        {
            spi_boost::throw_exception(std::bad_alloc());
        }

#endif

        r.release();
    }

#endif 

#if !defined( SPI_BOOST_NO_CXX11_SMART_PTR )

    template<class Y, class D>
    explicit shared_count( std::unique_ptr<Y, D> & r ): pi_( 0 )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
        typedef typename sp_convert_reference<D>::type D2;

        D2 d2( r.get_deleter() );
        pi_ = new sp_counted_impl_pd< typename std::unique_ptr<Y, D>::pointer, D2 >( r.get(), d2 );

#ifdef SPI_BOOST_NO_EXCEPTIONS

        if( pi_ == 0 )
        {
            spi_boost::throw_exception( std::bad_alloc() );
        }

#endif

        r.release();
    }

#endif

    template<class Y, class D>
    explicit shared_count( spi_boost::movelib::unique_ptr<Y, D> & r ): pi_( 0 )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
        typedef typename sp_convert_reference<D>::type D2;

        D2 d2( r.get_deleter() );
        pi_ = new sp_counted_impl_pd< typename spi_boost::movelib::unique_ptr<Y, D>::pointer, D2 >( r.get(), d2 );

#ifdef SPI_BOOST_NO_EXCEPTIONS

        if( pi_ == 0 )
        {
            spi_boost::throw_exception( std::bad_alloc() );
        }

#endif

        r.release();
    }

    ~shared_count() /*SPI_BOOST_SP_NOEXCEPT*/
    {
        if( pi_ != 0 ) pi_->release();
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        id_ = 0;
#endif
    }

    shared_count(shared_count const & r) SPI_BOOST_SP_NOEXCEPT: pi_(r.pi_)
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
        if( pi_ != 0 ) pi_->add_ref_copy();
    }

#if !defined( SPI_BOOST_NO_CXX11_RVALUE_REFERENCES )

    shared_count(shared_count && r) SPI_BOOST_SP_NOEXCEPT: pi_(r.pi_)
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
    {
        r.pi_ = 0;
    }

#endif

    explicit shared_count(weak_count const & r); // throws bad_weak_ptr when r.use_count() == 0
    shared_count( weak_count const & r, sp_nothrow_tag ) SPI_BOOST_SP_NOEXCEPT; // constructs an empty *this when r.use_count() == 0

    shared_count & operator= (shared_count const & r) SPI_BOOST_SP_NOEXCEPT
    {
        sp_counted_base * tmp = r.pi_;

        if( tmp != pi_ )
        {
            if( tmp != 0 ) tmp->add_ref_copy();
            if( pi_ != 0 ) pi_->release();
            pi_ = tmp;
        }

        return *this;
    }

    void swap(shared_count & r) SPI_BOOST_SP_NOEXCEPT
    {
        sp_counted_base * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const SPI_BOOST_SP_NOEXCEPT
    {
        return pi_ != 0? pi_->use_count(): 0;
    }

    bool unique() const SPI_BOOST_SP_NOEXCEPT
    {
        return use_count() == 1;
    }

    bool empty() const SPI_BOOST_SP_NOEXCEPT
    {
        return pi_ == 0;
    }

    friend inline bool operator==(shared_count const & a, shared_count const & b) SPI_BOOST_SP_NOEXCEPT
    {
        return a.pi_ == b.pi_;
    }

    friend inline bool operator<(shared_count const & a, shared_count const & b) SPI_BOOST_SP_NOEXCEPT
    {
        return std::less<sp_counted_base *>()( a.pi_, b.pi_ );
    }

    void * get_deleter( sp_typeinfo const & ti ) const SPI_BOOST_SP_NOEXCEPT
    {
        return pi_? pi_->get_deleter( ti ): 0;
    }

    void * get_local_deleter( sp_typeinfo const & ti ) const SPI_BOOST_SP_NOEXCEPT
    {
        return pi_? pi_->get_local_deleter( ti ): 0;
    }

    void * get_untyped_deleter() const SPI_BOOST_SP_NOEXCEPT
    {
        return pi_? pi_->get_untyped_deleter(): 0;
    }
};


class weak_count
{
private:

    sp_counted_base * pi_;

#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
    int id_;
#endif

    friend class shared_count;

public:

    SPI_BOOST_CONSTEXPR weak_count() SPI_BOOST_SP_NOEXCEPT: pi_(0)
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(weak_count_id)
#endif
    {
    }

    weak_count(shared_count const & r) SPI_BOOST_SP_NOEXCEPT: pi_(r.pi_)
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(weak_count_id)
#endif
    {
        if(pi_ != 0) pi_->weak_add_ref();
    }

    weak_count(weak_count const & r) SPI_BOOST_SP_NOEXCEPT: pi_(r.pi_)
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(weak_count_id)
#endif
    {
        if(pi_ != 0) pi_->weak_add_ref();
    }

// Move support

#if !defined( SPI_BOOST_NO_CXX11_RVALUE_REFERENCES )

    weak_count(weak_count && r) SPI_BOOST_SP_NOEXCEPT: pi_(r.pi_)
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(weak_count_id)
#endif
    {
        r.pi_ = 0;
    }

#endif

    ~weak_count() /*SPI_BOOST_SP_NOEXCEPT*/
    {
        if(pi_ != 0) pi_->weak_release();
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        id_ = 0;
#endif
    }

    weak_count & operator= (shared_count const & r) SPI_BOOST_SP_NOEXCEPT
    {
        sp_counted_base * tmp = r.pi_;

        if( tmp != pi_ )
        {
            if(tmp != 0) tmp->weak_add_ref();
            if(pi_ != 0) pi_->weak_release();
            pi_ = tmp;
        }

        return *this;
    }

    weak_count & operator= (weak_count const & r) SPI_BOOST_SP_NOEXCEPT
    {
        sp_counted_base * tmp = r.pi_;

        if( tmp != pi_ )
        {
            if(tmp != 0) tmp->weak_add_ref();
            if(pi_ != 0) pi_->weak_release();
            pi_ = tmp;
        }

        return *this;
    }

    void swap(weak_count & r) SPI_BOOST_SP_NOEXCEPT
    {
        sp_counted_base * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const SPI_BOOST_SP_NOEXCEPT
    {
        return pi_ != 0? pi_->use_count(): 0;
    }

    bool empty() const SPI_BOOST_SP_NOEXCEPT
    {
        return pi_ == 0;
    }

    friend inline bool operator==(weak_count const & a, weak_count const & b) SPI_BOOST_SP_NOEXCEPT
    {
        return a.pi_ == b.pi_;
    }

    friend inline bool operator<(weak_count const & a, weak_count const & b) SPI_BOOST_SP_NOEXCEPT
    {
        return std::less<sp_counted_base *>()(a.pi_, b.pi_);
    }
};

inline shared_count::shared_count( weak_count const & r ): pi_( r.pi_ )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
{
    if( pi_ == 0 || !pi_->add_ref_lock() )
    {
        spi_boost::throw_exception( spi_boost::bad_weak_ptr() );
    }
}

inline shared_count::shared_count( weak_count const & r, sp_nothrow_tag ) SPI_BOOST_SP_NOEXCEPT: pi_( r.pi_ )
#if defined(SPI_BOOST_SP_ENABLE_DEBUG_HOOKS)
        , id_(shared_count_id)
#endif
{
    if( pi_ != 0 && !pi_->add_ref_lock() )
    {
        pi_ = 0;
    }
}

} // namespace detail

} // namespace spi_boost

#if defined( SPI_BOOST_SP_DISABLE_DEPRECATED )
#pragma GCC diagnostic pop
#endif

#ifdef __BORLANDC__
# pragma warn .8027     // Functions containing try are not expanded inline
#endif

#endif  // #ifndef SPI_BOOST_SMART_PTR_DETAIL_SHARED_COUNT_HPP_INCLUDED
