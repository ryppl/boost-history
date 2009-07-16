#ifndef BOOST_SMART_PTR_GENERIC_SHARED_HPP_INCLUDED
#define BOOST_SMART_PTR_GENERIC_SHARED_HPP_INCLUDED

//
//  generic_shared.hpp
//
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001-2008 Peter Dimov
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/generic_shared.htm for documentation.
//

// requirements for generic pointer type:
// Must be either ordinary pointer, or provide:
//  operator->() and operator*()
//  value_type/reference/pointer member typedefs (or specialization of boost::smart_pointer_traits)
//  is_null_pointer() free function findable by ADL
//  (in)equality comparison
//  adl_static/const/dynamic_pointer_cast findable by ADL if you want support for
//   boost::static/const/dynamic_pointer_cast

#include <boost/config.hpp>   // for broken compiler workarounds

#if defined(BOOST_NO_MEMBER_TEMPLATES) && !defined(BOOST_MSVC6_MEMBER_TEMPLATES)
#include <boost/smart_ptr/detail/generic_shared_nmt.hpp>
#else

// In order to avoid circular dependencies with Boost.TR1
// we make sure that our include of <memory> doesn't try to
// pull in the TR1 headers: that's why we use this header
// rather than including <memory> directly:
#include <boost/config/no_tr1/memory.hpp>  // std::auto_ptr

#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/get_pointer.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/throw_exception.hpp>
#include <boost/smart_ptr/detail/shared_count.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/smart_ptr/detail/sp_convertible.hpp>

#if !defined(BOOST_SP_NO_ATOMIC_ACCESS)
#include <boost/smart_ptr/detail/spinlock_pool.hpp>
#include <boost/memory_order.hpp>
#endif

#include <algorithm>            // for std::swap
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

template<class T> class generic_shared;

template<class T> struct smart_pointer_traits
{
    typedef typename T::value_type value_type;
    typedef typename T::pointer pointer;
    typedef typename T::reference reference;
};

template<class T> struct smart_pointer_traits<T*>
{
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;
};

template<> struct smart_pointer_traits<void*>
{
    typedef void value_type;
    typedef void * pointer;
    typedef void reference;
};

template<> struct smart_pointer_traits<const void*>
{
    typedef void value_type;
    typedef const void * pointer;
    typedef void reference;
};

template<> struct smart_pointer_traits<volatile void*>
{
    typedef void value_type;
    typedef volatile void * pointer;
    typedef void reference;
};

template<> struct smart_pointer_traits<const volatile void*>
{
    typedef void value_type;
    typedef const volatile void * pointer;
    typedef void reference;
};

template<typename T> bool is_null_pointer(const generic_shared<T> &p)
{
    return !p;
}
template<typename T> bool is_null_pointer(T * p)
{
    return p == 0;
}

namespace gs_detail
{

struct static_cast_tag {};
struct const_cast_tag {};
struct dynamic_cast_tag {};

template<typename T, typename U, typename V>
inline void sp_enable_shared_from_this(T, U, V)
{
}

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
    detail::sp_enable_if_convertible<typename smart_pointer_traits<Y>::value_type, typename smart_pointer_traits<T>::value_type>
{};

template<typename T>
void set_plain_pointer_to_null(const T&)
{}
template<typename T>
void set_plain_pointer_to_null(T * &p)
{
	p = 0;
}

} // namespace gs_detail


//
//  generic_shared
//
//  An enhanced relative of scoped_ptr with reference counted copy semantics.
//  The object pointed to is deleted when the last generic_shared pointing to it
//  is destroyed or reset.
//

template<class T> class generic_shared
{
private:

    // Borland 5.5.1 specific workaround
    typedef generic_shared<T> this_type;

public:

    typedef typename boost::smart_pointer_traits<T>::value_type element_type;
    typedef typename boost::smart_pointer_traits<T>::value_type value_type;
    typedef T pointer;
    typedef typename boost::smart_pointer_traits<T>::reference reference;

    generic_shared(): px(), pn()
    {
    }

    template<class Y>
    explicit generic_shared( Y p ): px( p ), pn( p ) // Y must be complete
    {
        boost::gs_detail::sp_enable_shared_from_this( this, p, p );
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // generic_shared will release p by calling d(p)
    //

    template<class Y, class D> generic_shared(Y p, D d): px(p), pn(p, d)
    {
        boost::gs_detail::sp_enable_shared_from_this( this, p, p );
    }

    // As above, but with allocator. A's copy constructor shall not throw.

    template<class Y, class D, class A> generic_shared( Y p, D d, A a ): px( p ), pn( p, d, a )
    {
        boost::gs_detail::sp_enable_shared_from_this( this, p, p );
    }

//  generated copy constructor, destructor are fine

    template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

    generic_shared( generic_shared<Y> const & r, typename gs_detail::sp_enable_if_convertible<Y,T>::type = detail::sp_empty() )

#else

    generic_shared( generic_shared<Y> const & r )

#endif
    : px( r.px ), pn( r.pn ) // never throws
    {
    }

    // aliasing
    template< class Y >
    generic_shared( generic_shared<Y> const & r, T p ): px( p ), pn( r.pn ) // never throws
    {
    }

    template<class Y>
    generic_shared(generic_shared<Y> const & r, boost::gs_detail::static_cast_tag):
        px(adl_static_pointer_cast(r.px, boost::mpl::identity<pointer>())),
        pn(r.pn)
    {
    }

    template<class Y>
    generic_shared(generic_shared<Y> const & r, boost::gs_detail::const_cast_tag):
        px(adl_const_pointer_cast(r.px, boost::mpl::identity<pointer>())),
        pn(r.pn)
    {
    }

    template<class Y>
    generic_shared(generic_shared<Y> const & r, boost::gs_detail::dynamic_cast_tag):
        px(adl_dynamic_pointer_cast(r.px, boost::mpl::identity<pointer>())),
        pn(r.pn)
    {
        using boost::is_null_pointer;
        if(is_null_pointer(px)) // need to allocate new counter -- the cast failed
        {
            pn = boost::detail::shared_count();
        }
    }

#ifndef BOOST_NO_AUTO_PTR

    template<class Y>
    explicit generic_shared(std::auto_ptr<Y> & r): px(r.get()), pn()
    {
        Y * tmp = r.get();
        pn = boost::detail::shared_count(r);
        boost::gs_detail::sp_enable_shared_from_this( this, tmp, tmp );
    }

#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class Ap>
    explicit generic_shared( Ap r, typename boost::gs_detail::sp_enable_if_auto_ptr<Ap, int>::type = 0 ): px( r.get() ), pn()
    {
        typename Ap::element_type * tmp = r.get();
        pn = boost::detail::shared_count( r );
        boost::gs_detail::sp_enable_shared_from_this( this, tmp, tmp );
    }


#endif // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // BOOST_NO_AUTO_PTR

    // assignment

    generic_shared & operator=( generic_shared const & r ) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1400)

    template<class Y>
    generic_shared & operator=(generic_shared<Y> const & r) // never throws
    {
        this_type(r).swap(*this);
        return *this;
    }

#endif

#ifndef BOOST_NO_AUTO_PTR

    template<class Y>
    generic_shared & operator=( std::auto_ptr<Y> & r )
    {
        this_type(r).swap(*this);
        return *this;
    }

#if !defined( BOOST_NO_SFINAE ) && !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class Ap>
    typename boost::gs_detail::sp_enable_if_auto_ptr< Ap, generic_shared & >::type operator=( Ap r )
    {
        this_type( r ).swap( *this );
        return *this;
    }


#endif // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // BOOST_NO_AUTO_PTR

// Move support

#if defined( BOOST_HAS_RVALUE_REFS )

    generic_shared( generic_shared && r ): px( r.px ), pn() // never throws
    {
        pn.swap( r.pn );
        gs_detail::set_plain_pointer_to_null(r.px);
    }

    template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

    generic_shared( generic_shared<Y> && r, typename gs_detail::sp_enable_if_convertible<Y,T>::type = detail::sp_empty() )

#else

    generic_shared( generic_shared<Y> && r )

#endif
    : px( r.px ), pn() // never throws
    {
        pn.swap( r.pn );
        gs_detail::set_plain_pointer_to_null(r.px);
    }

    generic_shared & operator=( generic_shared && r ) // never throws
    {
        this_type( std::move( r ) ).swap( *this );
        return *this;
    }

    template<class Y>
    generic_shared & operator=( generic_shared<Y> && r ) // never throws
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

    template<class Y> void reset( generic_shared<Y> const & r, T p )
    {
        this_type( r, p ).swap( *this );
    }

    reference operator* () const // never throws
    {
        using boost::is_null_pointer;
        BOOST_ASSERT(!is_null_pointer(px));
        return *px;
    }

    pointer operator-> () const // never throws
    {
        using boost::is_null_pointer;
        BOOST_ASSERT(!is_null_pointer(px));
        return px;
    }

    pointer get() const // never throws
    {
        return px;
    }

// implicit conversion to "bool"
#if 0
#include <boost/smart_ptr/detail/generic_operator_bool.hpp>
#else
//  This header intentionally has no include guards.
//
//  Copyright (c) 2001-2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#if ( defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, < 0x570) ) || defined(__CINT__)

    operator bool () const
    {
        using boost::is_null_pointer;
        return !is_null_pointer(px);
    }

#elif defined( _MANAGED )

    static void unspecified_bool( this_type*** )
    {
    }

    typedef void (*unspecified_bool_type)( this_type*** );

    operator unspecified_bool_type() const // never throws
    {
        using boost::is_null_pointer;
        return is_null_pointer(px) ? 0: unspecified_bool;
    }

#elif \
    ( defined(__MWERKS__) && BOOST_WORKAROUND(__MWERKS__, < 0x3200) ) || \
    ( defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ < 304) ) || \
    ( defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, <= 0x590) )

    typedef T (this_type::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const // never throws
    {
        using boost::is_null_pointer;
        return is_null_pointer(px) ? 0: &this_type::get;
    }

#else

    typedef T this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const // never throws
    {
        using boost::is_null_pointer;
        return is_null_pointer(px) ? 0: &this_type::px;
    }

#endif

    // operator! is redundant, but some compilers need it
    bool operator! () const // never throws
    {
        using boost::is_null_pointer;
        return is_null_pointer(px);
    }
#endif // end implicit conversion to "bool" support

    bool unique() const // never throws
    {
        return pn.unique();
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    void swap(generic_shared<T> & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    template<class Y> bool _internal_less(generic_shared<Y> const & rhs) const
    {
        return pn < rhs.pn;
    }

    void * _internal_get_deleter( detail::sp_typeinfo const & ti ) const
    {
        return pn.get_deleter( ti );
    }

    bool _internal_equiv( generic_shared const & r ) const
    {
        return px == r.px && pn == r.pn;
    }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<class Y> friend class generic_shared;
    template<class Y> friend class weak_ptr;


#endif

    T px;                     // contained pointer
    boost::detail::shared_count pn;    // reference counter

};  // generic_shared

template<class T, class U> inline bool operator==(generic_shared<T> const & a, generic_shared<U> const & b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(generic_shared<T> const & a, generic_shared<U> const & b)
{
    return a.get() != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<class T> inline bool operator!=(generic_shared<T> const & a, generic_shared<T> const & b)
{
    return a.get() != b.get();
}

#endif

template<class T, class U> inline bool operator<(generic_shared<T> const & a, generic_shared<U> const & b)
{
    return a._internal_less(b);
}

template<class T> inline void swap(generic_shared<T> & a, generic_shared<T> & b)
{
    a.swap(b);
}

template<class T, class U> generic_shared<T> static_pointer_cast(generic_shared<U> const & r)
{
    return adl_static_pointer_cast(r, boost::mpl::identity<generic_shared<T> >());
}

template<class T, class U> generic_shared<T> const_pointer_cast(generic_shared<U> const & r)
{
    return adl_const_pointer_cast(r, boost::mpl::identity<generic_shared<T> >());
}

template<class T, class U> generic_shared<T> dynamic_pointer_cast(generic_shared<U> const & r)
{
    return adl_dynamic_pointer_cast(r, boost::mpl::identity<generic_shared<T> >());
}

template<typename T, typename U>
generic_shared<T> adl_static_pointer_cast(generic_shared<U> const & r, boost::mpl::identity<generic_shared<T> >)
{
    return generic_shared<T>(r, boost::gs_detail::static_cast_tag());
}

template<typename T, typename U>
generic_shared<T> adl_const_pointer_cast(generic_shared<U> const & r, boost::mpl::identity<generic_shared<T> >)
{
    return generic_shared<T>(r, boost::gs_detail::const_cast_tag());
}

template<typename T, typename U>
generic_shared<T> adl_dynamic_pointer_cast(generic_shared<U> const & r, boost::mpl::identity<generic_shared<T> >)
{
    return generic_shared<T>(r, boost::gs_detail::dynamic_cast_tag());
}

template<typename T, typename U>
T* adl_static_pointer_cast(U *r, boost::mpl::identity<T*>)
{
    return static_cast<T*>(r);
}

template<typename T, typename U>
T* adl_const_pointer_cast(U *r, boost::mpl::identity<T*>)
{
    return const_cast<T*>(r);
}

template<typename T, typename U>
T* adl_dynamic_pointer_cast(U *r, boost::mpl::identity<T*>)
{
    return dynamic_cast<T*>(r);
}

// get_pointer() enables boost::mem_fn to recognize generic_shared
template<class T> inline T get_pointer(generic_shared<T> const & p)
{
    return p.get();
}

// operator<<

#if !defined(BOOST_NO_IOSTREAM)

#if defined(BOOST_NO_TEMPLATED_IOSTREAMS) || ( defined(__GNUC__) &&  (__GNUC__ < 3) )

template<class Y> std::ostream & operator<< (std::ostream & os, generic_shared<Y> const & p)
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
template<class E, class T, class Y> basic_ostream<E, T> & operator<< (basic_ostream<E, T> & os, generic_shared<Y> const & p)
# else
template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, generic_shared<Y> const & p)
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

template<class D, class T> D * get_deleter(generic_shared<T> const & p)
{
    void const * q = p._internal_get_deleter(BOOST_SP_TYPEID(D));
    return const_cast<D *>(static_cast<D const *>(q));
}

#else

template<class D, class T> D * get_deleter(generic_shared<T> const & p)
{
    return static_cast<D *>(p._internal_get_deleter(BOOST_SP_TYPEID(D)));
}

#endif

// atomic access

#if !defined(BOOST_SP_NO_ATOMIC_ACCESS)

template<class T> inline bool atomic_is_lock_free( generic_shared<T> const * /*p*/ )
{
    return false;
}

template<class T> generic_shared<T> atomic_load( generic_shared<T> const * p )
{
    boost::detail::spinlock_pool<2>::scoped_lock lock( p );
    return *p;
}

template<class T> inline generic_shared<T> atomic_load_explicit( generic_shared<T> const * p, memory_order /*mo*/ )
{
    return atomic_load( p );
}

template<class T> void atomic_store( generic_shared<T> * p, generic_shared<T> r )
{
    boost::detail::spinlock_pool<2>::scoped_lock lock( p );
    p->swap( r );
}

template<class T> inline void atomic_store_explicit( generic_shared<T> * p, generic_shared<T> r, memory_order /*mo*/ )
{
    atomic_store( p, r ); // std::move( r )
}

template<class T> generic_shared<T> atomic_exchange( generic_shared<T> * p, generic_shared<T> r )
{
    boost::detail::spinlock & sp = boost::detail::spinlock_pool<2>::spinlock_for( p );

    sp.lock();
    p->swap( r );
    sp.unlock();

    return r; // return std::move( r )
}

template<class T> generic_shared<T> atomic_exchange_explicit( generic_shared<T> * p, generic_shared<T> r, memory_order /*mo*/ )
{
    return atomic_exchange( p, r ); // std::move( r )
}

template<class T> bool atomic_compare_exchange( generic_shared<T> * p, generic_shared<T> * v, generic_shared<T> w )
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
        generic_shared<T> tmp( *p );

        sp.unlock();

        tmp.swap( *v );
        return false;
    }
}

template<class T> inline bool atomic_compare_exchange_explicit( generic_shared<T> * p, generic_shared<T> * v, generic_shared<T> w, memory_order /*success*/, memory_order /*failure*/ )
{
    return atomic_compare_exchange( p, v, w ); // std::move( w )
}

#endif

} // namespace boost

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

#endif  // #if defined(BOOST_NO_MEMBER_TEMPLATES) && !defined(BOOST_MSVC6_MEMBER_TEMPLATES)

#endif  // #ifndef BOOST_SMART_PTR_SHARED_HPP_INCLUDED
