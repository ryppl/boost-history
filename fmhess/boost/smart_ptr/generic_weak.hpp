#ifndef BOOST_SMART_PTR_GENERIC_WEAK_HPP_INCLUDED
#define BOOST_SMART_PTR_GENERIC_WEAK_HPP_INCLUDED

//
//  generic_weak.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov
//  Copyright (c) 2009 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/generic_weak.htm for documentation.
//

#include <memory> // boost.TR1 include order fix
#include <boost/smart_ptr/detail/shared_count.hpp>
#include <boost/smart_ptr/generic_shared.hpp>

#ifdef BOOST_MSVC  // moved here to work around VC++ compiler crash
# pragma warning(push)
# pragma warning(disable:4284) // odd return type for operator->
#endif

namespace boost
{

template<class T> class generic_weak
{
private:

    // Borland 5.5.1 specific workarounds
    typedef generic_weak<T> this_type;

public:

    typedef typename generic_pointer_traits<T>::value_type element_type;
    typedef typename generic_pointer_traits<T>::value_type value_type;
    typedef generic_shared<T> shared_type;

    generic_weak(): px(), pn() // never throws in 1.30+
    {
    }

//  generated copy constructor, assignment, destructor are fine


//
//  The "obvious" converting constructor implementation:
//
//  template<class Y>
//  generic_weak(generic_weak<Y> const & r): px(r.px), pn(r.pn) // never throws
//  {
//  }
//
//  has a serious problem.
//
//  r.px may already have been invalidated. The px(r.px)
//  conversion may require access to *r.px (virtual inheritance).
//
//  It is not possible to avoid spurious access violations since
//  in multithreaded programs r.px may be invalidated at any point.
//

    template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

    generic_weak( generic_weak<Y> const & r, typename gs_detail::sp_enable_if_convertible<Y,T>::type = detail::sp_empty() )

#else

    generic_weak( generic_weak<Y> const & r )

#endif
    : px(r.lock().get()), pn(r.pn) // never throws
    {
    }

#if defined( BOOST_HAS_RVALUE_REFS )

    template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

    generic_weak( generic_weak<Y> && r, typename gs_detail::sp_enable_if_convertible<Y,T>::type = detail::sp_empty() )

#else

    generic_weak( generic_weak<Y> && r )

#endif
    : px(r.lock().get()), pn(std::move(r.pn)) // never throws
    {
        gs_detail::set_plain_pointer_to_null(r.px);
    }

    // for better efficiency in the T == Y case
    generic_weak( generic_weak && r ): px( r.px ), pn(std::move(r.pn)) // never throws
    {
        gs_detail::set_plain_pointer_to_null(r.px);
    }

    // for better efficiency in the T == Y case
    generic_weak & operator=( generic_weak && r ) // never throws
    {
        this_type( std::move( r ) ).swap( *this );
        return *this;
    }


#endif

    template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

    generic_weak( generic_shared<Y> const & r, typename gs_detail::sp_enable_if_convertible<Y,T>::type = detail::sp_empty() )

#else

    generic_weak( generic_shared<Y> const & r )

#endif
    : px( r.px ), pn( r.pn ) // never throws
    {
    }

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)

    template<class Y>
    generic_weak & operator=(generic_weak<Y> const & r) // never throws
    {
        px = r.lock().get();
        pn = r.pn;
        return *this;
    }

#if defined( BOOST_HAS_RVALUE_REFS )

    template<class Y>
    generic_weak & operator=(generic_weak<Y> && r)
    {
        this_type( std::move( r ) ).swap( *this );
        return *this;
    }

#endif

    template<class Y>
    generic_weak & operator=(generic_shared<Y> const & r) // never throws
    {
        px = r.px;
        pn = r.pn;
        return *this;
    }

#endif

    generic_shared<T> lock() const // never throws
    {
        return generic_shared<T>( *this, boost::detail::sp_nothrow_tag() );
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    bool expired() const // never throws
    {
        return pn.use_count() == 0;
    }

    bool _empty() const // extension, not in std::weak_ptr
    {
        return pn.empty();
    }

    void reset() // never throws in 1.30+
    {
        this_type().swap(*this);
    }

    void swap(this_type & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    void _internal_assign(T px2, boost::detail::shared_count const & pn2)
    {
        px = px2;
        pn = pn2;
    }

    template<class Y> bool _internal_less(generic_weak<Y> const & rhs) const
    {
        return pn < rhs.pn;
    }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<class Y> friend class generic_weak;
    template<class Y> friend class generic_shared;

#endif

    T px;                       // contained pointer
    boost::detail::weak_count pn; // reference counter

};  // generic_weak

template<class T, class U> inline bool operator<(generic_weak<T> const & a, generic_weak<U> const & b)
{
    return a._internal_less(b);
}

template<class T> void swap(generic_weak<T> & a, generic_weak<T> & b)
{
    a.swap(b);
}

} // namespace boost

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

#endif  // #ifndef BOOST_SMART_PTR_GENERIC_WEAK_HPP_INCLUDED
