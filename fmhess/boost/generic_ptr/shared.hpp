#ifndef BOOST_GENERIC_PTR_SHARED_HPP_INCLUDED
#define BOOST_GENERIC_PTR_SHARED_HPP_INCLUDED

//
//  generic_ptr/shared.hpp
//
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001-2008 Peter Dimov
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

// requirements for generic pointer type:
// Must be either ordinary pointer, or provide:
//  operator->() and operator*()
//  value_type/reference/pointer member typedefs (or specialization of boost::generic_ptr::pointer_traits)
//  (in)equality comparison
//  2 argument static/const/dynamic_pointer_cast findable by ADL if you want support for casting
//  get_pointer support
//  rebind member template for changing a pointer's value_type

#include <boost/config.hpp>   // for broken compiler workarounds

#if defined(BOOST_NO_MEMBER_TEMPLATES) && !defined(BOOST_MSVC6_MEMBER_TEMPLATES)
#include <boost/smart_ptr/detail/generic_shared_nmt.hpp>
#else

// In order to avoid circular dependencies with Boost.TR1
// we make sure that our include of <memory> doesn't try to
// pull in the TR1 headers: that's why we use this header
// rather than including <memory> directly:
#include <boost/config/no_tr1/memory.hpp>  // std::auto_ptr

#include <boost/checked_delete.hpp>
#include <boost/generic_ptr/detail/util.hpp>
#include <boost/generic_ptr/pointer_cast.hpp>
#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/throw_exception.hpp>
#include <boost/smart_ptr/detail/shared_count.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/smart_ptr/detail/sp_convertible.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/swap.hpp>

#if !defined(BOOST_SP_NO_ATOMIC_ACCESS)
#include <boost/smart_ptr/detail/spinlock_pool.hpp>
#include <boost/memory_order.hpp>
#endif

#include <functional>           // for std::less
#include <typeinfo>             // for std::bad_cast

#if !defined(BOOST_NO_IOSTREAM)
#if !defined(BOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif

#ifdef BOOST_MSVC  // moved here to work around VC++ compiler crash
# pragma warning(push)
# pragma warning(disable:4284) // odd return type for operator->
#endif

namespace boost
{
namespace generic_ptr
{

template<typename T> class shared;
template<typename T> class weak;
template<typename T> class enable_shared_from_this;

namespace detail
{

// enable_shared_from_this support

template< class X, class Y, class T > inline void sp_enable_shared_from_this( shared<X> const * ppx, Y const * py,
    enable_shared_from_this< T > const * pe )
{
    if( pe != 0 )
    {
        typedef typename remove_const<typename pointer_traits<Y>::value_type>::type nonconst_y_value_type;
        pe->_internal_accept_owner( ppx, const_pointer_cast( *py, mpl::identity<nonconst_y_value_type>()) );
    }
}

#ifdef _MANAGED

// Avoid C4793, ... causes native code generation

struct sp_any_pointer
{
    template<class T> sp_any_pointer( T* ) {}
};

inline void sp_enable_shared_from_this( sp_any_pointer, sp_any_pointer, sp_any_pointer )
{
}

#else // _MANAGED

inline void sp_enable_shared_from_this( ... )
{
}

#endif // _MANAGED

#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION ) && !defined( BOOST_NO_AUTO_PTR )

// rvalue auto_ptr support based on a technique by Dave Abrahams

template< class T, class R > struct sp_enable_if_auto_ptr
{
};

template< class T, class R > struct sp_enable_if_auto_ptr< std::auto_ptr< T >, R >
{
    typedef R type;
};

#endif

template<typename Y, typename T>
class sp_enable_if_convertible: public
    boost::detail::sp_enable_if_convertible<typename pointer_traits<Y>::value_type, typename pointer_traits<T>::value_type>
{};

} // namespace detail


//
//  generic_ptr::shared
//
//  A generalization of boost::shared_ptr which accepts a generic pointer type as
//  its wrapped pointer.

template<class T> class shared
{
private:

    // Borland 5.5.1 specific workaround
    typedef shared<T> this_type;

public:

    typedef typename pointer_traits<T>::value_type value_type;
    typedef value_type element_type;
    typedef T pointer;
    typedef typename pointer_traits<T>::reference reference;
    typedef weak<T> weak_type;

    template<typename ValueType>
    struct rebind
    {
        typedef shared<typename generic_ptr::rebind<pointer, ValueType>::other > other;
    };

    shared(): px(), pn()
    {
    }

    template<class Y>
    explicit shared( Y p ): px( p ), pn( get_plain_old_pointer(p) ) // Y must be complete
    {
        detail::sp_enable_shared_from_this( this, &p, get_plain_old_pointer(p) );
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // shared will release p by calling d(p)
    //

    template<class Y, class D> shared(Y p, D d): px(p), pn(p, d)
    {
        detail::sp_enable_shared_from_this( this, &p, get_plain_old_pointer(p) );
    }

    // As above, but with allocator. A's copy constructor shall not throw.

    template<class Y, class D, class A> shared( Y p, D d, A a ): px( p ), pn( p, d, a )
    {
        detail::sp_enable_shared_from_this( this, &p, get_plain_old_pointer(p) );
    }

//  generated copy constructor, destructor are fine

    template<class Y>
    explicit shared(weak<Y> const & r): pn(r.pn) // may throw
    {
        // it is now safe to copy r.px, as pn(r.pn) did not throw
        px = r.px;
    }

    template<class Y>
    shared( weak<Y> const & r, boost::detail::sp_nothrow_tag ): px( 0 ), pn( r.pn, boost::detail::sp_nothrow_tag() ) // never throws
    {
        if( !pn.empty() )
        {
            px = r.px;
        }
    }

    template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

    shared( shared<Y> const & r, typename detail::sp_enable_if_convertible<Y,T>::type = boost::detail::sp_empty() )

#else

    shared( shared<Y> const & r )

#endif
    : px( r.px ), pn( r.pn ) // never throws
    {
    }

    // aliasing
    template< class Y, class Z >
    shared( shared<Y> const & r, Z p ): px( p ), pn( r.pn ) // never throws
    {
    }

    template<class Y>
    shared(shared<Y> const & r, detail::static_cast_tag):
        px(static_pointer_cast(r.px, boost::mpl::identity<value_type>())),
        pn(r.pn)
    {
    }

    template<class Y>
    shared(shared<Y> const & r, detail::const_cast_tag):
        px(const_pointer_cast(r.px, boost::mpl::identity<value_type>())),
        pn(r.pn)
    {
    }

    template<class Y>
    shared(shared<Y> const & r, detail::dynamic_cast_tag):
        px(dynamic_pointer_cast(r.px, boost::mpl::identity<value_type>())),
        pn(r.pn)
    {
        if(get_plain_old_pointer(px) == 0) // need to allocate new counter -- the cast failed
        {
            pn = boost::detail::shared_count();
        }
    }

#ifndef BOOST_NO_AUTO_PTR

    template<class Y>
    explicit shared(std::auto_ptr<Y> & r): px(r.get()), pn()
    {
        Y * tmp = r.get();
        pn = boost::detail::shared_count(r);
        detail::sp_enable_shared_from_this( this, &tmp, get_plain_old_pointer(tmp) );
    }

#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class Ap>
    explicit shared( Ap r, typename detail::sp_enable_if_auto_ptr<Ap, int>::type = 0 ): px( r.get() ), pn()
    {
        typename Ap::element_type * tmp = r.get();
        pn = boost::detail::shared_count( r );
        detail::sp_enable_shared_from_this( this, &tmp, get_plain_old_pointer(tmp) );
    }


#endif // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // BOOST_NO_AUTO_PTR

    // assignment

    shared & operator=( shared const & r ) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1400)

    template<class Y>
    shared & operator=(shared<Y> const & r) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

#endif

#ifndef BOOST_NO_AUTO_PTR

    template<class Y>
    shared & operator=( std::auto_ptr<Y> & r )
    {
        this_type(r).swap(*this);
        return *this;
    }

#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class Ap>
    typename detail::sp_enable_if_auto_ptr< Ap, shared & >::type operator=( Ap r )
    {
        this_type( r ).swap( *this );
        return *this;
    }


#endif // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // BOOST_NO_AUTO_PTR

// Move support

#ifndef BOOST_NO_RVALUE_REFERENCES

    shared( shared && r ): px( std::move(r.px) ), pn() // never throws
    {
        pn.swap( r.pn );
        detail::set_plain_old_pointer_to_null(r.px);
    }

    template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

    shared( shared<Y> && r, typename detail::sp_enable_if_convertible<Y,T>::type = boost::detail::sp_empty() )

#else

    shared( shared<Y> && r )

#endif
    : px( std::move(r.px) ), pn() // never throws
    {
        pn.swap( r.pn );
        detail::set_plain_old_pointer_to_null(r.px);
    }

    shared & operator=( shared && r ) // never throws
    {
        this_type( std::move( r ) ).swap( *this );
        return *this;
    }

    template<class Y>
    shared & operator=( shared<Y> && r ) // never throws
    {
        this_type( std::move( r ) ).swap( *this );
        return *this;
    }

#endif

    void reset() // never throws in 1.30+
    {
        this_type().swap(*this);
    }

    template<class Y> void reset(Y p) // Y must be complete
    {
        this_type(p).swap(*this);
    }

    template<class Y, class D> void reset( Y p, D d )
    {
        this_type( p, d ).swap( *this );
    }

    template<class Y, class D, class A> void reset( Y p, D d, A a )
    {
        this_type( p, d, a ).swap( *this );
    }

    template<class Y> void reset( shared<Y> const & r, T p )
    {
        this_type( r, p ).swap( *this );
    }

    reference operator* () const // never throws
    {
        return *px;
    }

    pointer operator-> () const // never throws
    {
        return px;
    }

    pointer get() const // never throws
    {
        return px;
    }

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

    bool unique() const // never throws
    {
        return pn.unique();
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    void swap(shared<T> & other) // never throws
    {
        boost::swap(px, other.px);
        pn.swap(other.pn);
    }

    template<class Y> bool _internal_less(shared<Y> const & rhs) const
    {
        return pn < rhs.pn;
    }

    void * _internal_get_deleter( boost::detail::sp_typeinfo const & ti ) const
    {
        return pn.get_deleter( ti );
    }

    bool _internal_equiv( shared const & r ) const
    {
        return px == r.px && pn == r.pn;
    }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<class Y> friend class shared;
    template<class Y> friend class weak;


#endif

    T px;                     // contained pointer
    boost::detail::shared_count pn;    // reference counter

};  // shared

template<class T, class U> inline bool operator==(shared<T> const & a, shared<U> const & b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(shared<T> const & a, shared<U> const & b)
{
    return a.get() != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<class T> inline bool operator!=(shared<T> const & a, shared<T> const & b)
{
    return a.get() != b.get();
}

#endif

template<class T, class U> inline bool operator<(shared<T> const & a, shared<U> const & b)
{
    return a._internal_less(b);
}

template<class T> inline void swap(shared<T> & a, shared<T> & b)
{
    a.swap(b);
}

template<typename ToValueType, typename U>
typename shared<U>::template rebind<ToValueType>::other static_pointer_cast(shared<U> const & r,
    boost::mpl::identity<ToValueType> = boost::mpl::identity<ToValueType>())
{
    typedef typename shared<U>::template rebind<ToValueType>::other result_type;
    return result_type(r, detail::static_cast_tag());
}

template<typename ToValueType, typename U>
typename shared<U>::template rebind<ToValueType>::other const_pointer_cast(shared<U> const & r,
    boost::mpl::identity<ToValueType> = boost::mpl::identity<ToValueType>())
{
    typedef typename shared<U>::template rebind<ToValueType>::other result_type;
    return result_type(r, detail::const_cast_tag());
}

template<typename ToValueType, typename U>
typename shared<U>::template rebind<ToValueType>::other dynamic_pointer_cast(shared<U> const & r,
    boost::mpl::identity<ToValueType> = boost::mpl::identity<ToValueType>())
{
    typedef typename shared<U>::template rebind<ToValueType>::other result_type;
    return result_type(r, detail::dynamic_cast_tag());
}

// get_pointer() enables boost::mem_fn to recognize generic_ptr::shared
// and is required by generic_ptr::get_plain_old_pointer()
template<class T> inline T get_pointer(shared<T> const & p)
{
    return p.get();
}

// operator<<

#if !defined(BOOST_NO_IOSTREAM)

#if defined(BOOST_NO_TEMPLATED_IOSTREAMS) || ( defined(__GNUC__) &&  (__GNUC__ < 3) )

template<class Y> std::ostream & operator<< (std::ostream & os, shared<Y> const & p)
{
    os << p.get();
    return os;
}

#else

// in STLport's no-iostreams mode no iostream symbols can be used
#ifndef _STLP_NO_IOSTREAMS

# if defined(BOOST_MSVC) && BOOST_WORKAROUND(BOOST_MSVC, < 1300 && __SGI_STL_PORT)
// MSVC6 has problems finding std::basic_ostream through the using declaration in namespace _STL
using std::basic_ostream;
template<class E, class T, class Y> basic_ostream<E, T> & operator<< (basic_ostream<E, T> & os, shared<Y> const & p)
# else
template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, shared<Y> const & p)
# endif
{
    os << p.get();
    return os;
}

#endif // _STLP_NO_IOSTREAMS

#endif // __GNUC__ < 3

#endif // !defined(BOOST_NO_IOSTREAM)

// get_deleter

#if ( defined(__GNUC__) && BOOST_WORKAROUND(__GNUC__, < 3) ) || \
    ( defined(__EDG_VERSION__) && BOOST_WORKAROUND(__EDG_VERSION__, <= 238) ) || \
    ( defined(__HP_aCC) && BOOST_WORKAROUND(__HP_aCC, <= 33500) )

// g++ 2.9x doesn't allow static_cast<X const *>(void *)
// apparently EDG 2.38 and HP aCC A.03.35 also don't accept it

template<class D, class T> D * get_deleter(shared<T> const & p)
{
    void const * q = p._internal_get_deleter(BOOST_SP_TYPEID(D));
    return const_cast<D *>(static_cast<D const *>(q));
}

#else

template<class D, class T> D * get_deleter(shared<T> const & p)
{
    return static_cast<D *>(p._internal_get_deleter(BOOST_SP_TYPEID(D)));
}

#endif

// atomic access

#if !defined(BOOST_SP_NO_ATOMIC_ACCESS)

template<class T> inline bool atomic_is_lock_free( shared<T> const * /*p*/ )
{
    return false;
}

template<class T> shared<T> atomic_load( shared<T> const * p )
{
    boost::detail::spinlock_pool<2>::scoped_lock lock( p );
    return *p;
}

template<class T> inline shared<T> atomic_load_explicit( shared<T> const * p, memory_order /*mo*/ )
{
    return atomic_load( p );
}

template<class T> void atomic_store( shared<T> * p, shared<T> r )
{
    boost::detail::spinlock_pool<2>::scoped_lock lock( p );
    p->swap( r );
}

template<class T> inline void atomic_store_explicit( shared<T> * p, shared<T> r, memory_order /*mo*/ )
{
    atomic_store( p, r ); // std::move( r )
}

template<class T> shared<T> atomic_exchange( shared<T> * p, shared<T> r )
{
    boost::detail::spinlock & sp = boost::detail::spinlock_pool<2>::spinlock_for( p );

    sp.lock();
    p->swap( r );
    sp.unlock();

    return r; // return std::move( r )
}

template<class T> shared<T> atomic_exchange_explicit( shared<T> * p, shared<T> r, memory_order /*mo*/ )
{
    return atomic_exchange( p, r ); // std::move( r )
}

template<class T> bool atomic_compare_exchange( shared<T> * p, shared<T> * v, shared<T> w )
{
    boost::detail::spinlock & sp = boost::detail::spinlock_pool<2>::spinlock_for( p );

    sp.lock();

    if( p->_internal_equiv( *v ) )
    {
        p->swap( w );

        sp.unlock();

        return true;
    }
    else
    {
        shared<T> tmp( *p );

        sp.unlock();

        tmp.swap( *v );
        return false;
    }
}

template<class T> inline bool atomic_compare_exchange_explicit( shared<T> * p, shared<T> * v, shared<T> w, memory_order /*success*/, memory_order /*failure*/ )
{
    return atomic_compare_exchange( p, v, w ); // std::move( w )
}

#endif

} // namespace generic_ptr
} // namespace boost

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

#endif  // #if defined(BOOST_NO_MEMBER_TEMPLATES) && !defined(BOOST_MSVC6_MEMBER_TEMPLATES)

#endif  // #ifndef BOOST_GENERIC_PTR_SHARED_HPP_INCLUDED
