#ifndef BOOST_GENERIC_PTR_INTRUSIVE_PTR_INCLUDED
#define BOOST_GENERIC_PTR_INTRUSIVE_PTR_INCLUDED

//
//  generic_ptr/intrusive.hpp
//
//  Copyright (c) 2001, 2002 Peter Dimov
//  Copyright (c) 2009 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#include <boost/config.hpp>

#ifdef BOOST_MSVC  // moved here to work around VC++ compiler crash
# pragma warning(push)
# pragma warning(disable:4284) // odd return type for operator->
#endif

#include <boost/assert.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/generic_ptr/detail/util.hpp>
#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/smart_ptr/detail/sp_convertible.hpp>
#include <boost/utility/swap.hpp>

#include <boost/config/no_tr1/functional.hpp>           // for std::less

#if !defined(BOOST_NO_IOSTREAM)
#if !defined(BOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif


namespace boost
{
namespace generic_ptr
{
//
//  generic_ptr::intrusive
//
//  A generalization of intrusive_ptr which can use a generic pointer type.
//

template<class T> class intrusive
{
private:

    typedef intrusive this_type;

public:

    typedef typename pointer_traits<T>::value_type value_type;
    typedef value_type element_type;
    typedef T pointer;
    typedef typename pointer_traits<T>::reference reference;

    intrusive(): px()
    {
    }


    intrusive( T p, bool add_ref = true ): px( p )
    {
        if( get_plain_old_pointer(px) != 0 && add_ref ) intrusive_ptr_add_ref( get_plain_old_pointer(px) );
    }

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)

    template<class U>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

    intrusive( intrusive<U> const & rhs, typename boost::detail::sp_enable_if_convertible
        <
            typename pointer_traits<U>::value_type,
            typename pointer_traits<T>::value_type
        >::type = boost::detail::sp_empty() )

#else

    intrusive( intrusive<U> const & rhs )

#endif
    : px( rhs.get() )
    {
        if( get_plain_old_pointer(px) != 0 ) intrusive_ptr_add_ref( get_plain_old_pointer(px) );
    }

#endif

    intrusive(intrusive const & rhs): px( rhs.px )
    {
        if( get_plain_old_pointer(px) != 0 ) intrusive_ptr_add_ref( get_plain_old_pointer(px) );
    }

    ~intrusive()
    {
        if( get_plain_old_pointer(px) != 0 ) intrusive_ptr_release( get_plain_old_pointer(px) );
    }

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)

    template<class U> intrusive & operator=(intrusive<U> const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

#endif

// Move support

#if defined( BOOST_HAS_RVALUE_REFS )

    intrusive(intrusive && rhs): px( rhs.px )
    {
        detail::set_plain_old_pointer_to_null(px);
    }

    intrusive & operator=(intrusive && rhs)
    {
        this_type(std::move(rhs)).swap(*this);
        return *this;
    }

#endif

    intrusive & operator=(intrusive const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    intrusive & operator=(T rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    void reset()
    {
        this_type().swap( *this );
    }

    void reset( T rhs )
    {
        this_type( rhs ).swap( *this );
    }

    pointer get() const
    {
        return px;
    }

    reference operator*() const
    {
        BOOST_ASSERT( get_plain_old_pointer(px) != 0 );
        return *px;
    }

    pointer operator->() const
    {
        BOOST_ASSERT( get_plain_old_pointer(px) != 0 );
        return px;
    }

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

    void swap(intrusive & rhs)
    {
        boost::swap(px, rhs.px);
    }

private:

    T px;
};

template<class T, class U> inline bool operator==(intrusive<T> const & a, intrusive<U> const & b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(intrusive<T> const & a, intrusive<U> const & b)
{
    return a.get() != b.get();
}

template<class T, class U> inline bool operator==(intrusive<T> const & a, U * b)
{
    return a.get() == b;
}

template<class T, class U> inline bool operator!=(intrusive<T> const & a, U * b)
{
    return a.get() != b;
}

template<class T, class U> inline bool operator==(T * a, intrusive<U> const & b)
{
    return a == b.get();
}

template<class T, class U> inline bool operator!=(T * a, intrusive<U> const & b)
{
    return a != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<class T> inline bool operator!=(intrusive<T> const & a, intrusive<T> const & b)
{
    return a.get() != b.get();
}

#endif

template<class T> inline bool operator<(intrusive<T> const & a, intrusive<T> const & b)
{
    return std::less<T *>()(a.get(), b.get());
}

template<class T> void swap(intrusive<T> & lhs, intrusive<T> & rhs)
{
    lhs.swap(rhs);
}

// mem_fn support

template<class T> T get_pointer(intrusive<T> const & p)
{
    return p.get();
}

template<class ToValueType, class U>
typename rebind<intrusive<U>, ToValueType>::other static_pointer_cast
(
  intrusive<U> const & p,
  mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
)
{
    return static_pointer_cast(p.get(), to_type_iden);
}

template<class ToValueType, class U>
typename rebind<intrusive<U>, ToValueType>::other const_pointer_cast
(
  intrusive<U> const & p,
  mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
)
{
    return const_pointer_cast(p.get(), to_type_iden);
}

template<class ToValueType, class U>
typename rebind<intrusive<U>, ToValueType>::other dynamic_pointer_cast
(
  intrusive<U> const & p,
  mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
)
{
    return dynamic_pointer_cast(p.get(), to_type_iden);
}

// operator<<

#if !defined(BOOST_NO_IOSTREAM)

#if defined(BOOST_NO_TEMPLATED_IOSTREAMS) || ( defined(__GNUC__) &&  (__GNUC__ < 3) )

template<class Y> std::ostream & operator<< (std::ostream & os, intrusive<Y> const & p)
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
template<class E, class T, class Y> basic_ostream<E, T> & operator<< (basic_ostream<E, T> & os, intrusive<Y> const & p)
# else
template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, intrusive<Y> const & p)
# endif
{
    os << p.get();
    return os;
}

#endif // _STLP_NO_IOSTREAMS

#endif // __GNUC__ < 3

#endif // !defined(BOOST_NO_IOSTREAM)

} // namespace generic_ptr
} // namespace boost

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

#endif  // #ifndef BOOST_GENERIC_PTR_INTRUSIVE_HPP_INCLUDED
