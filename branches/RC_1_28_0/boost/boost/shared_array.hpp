#ifndef BOOST_SHARED_ARRAY_HPP_INCLUDED
#define BOOST_SHARED_ARRAY_HPP_INCLUDED

//
//  shared_array.hpp
//
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/smart_ptr/shared_array.htm for documentation.
//

#include <boost/config.hpp>   // for broken compiler workarounds

#ifndef BOOST_MSVC6_MEMBER_TEMPLATES
#include <boost/detail/shared_array_nmt.hpp>
#else

#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>

#include <boost/detail/shared_count.hpp>

#include <cstddef>            // for std::ptrdiff_t
#include <algorithm>          // for std::swap
#include <functional>         // for std::less

namespace boost
{

//
//  shared_array
//
//  shared_array extends shared_ptr to arrays.
//  The array pointed to is deleted when the last shared_array pointing to it
//  is destroyed or reset.
//

template<typename T> class shared_array
{
private:

    // Borland 5.5.1 specific workarounds
    typedef checked_array_deleter<T> deleter;
    typedef shared_array<T> this_type;

public:

    typedef T element_type;

    explicit shared_array(T * p = 0): px(p), pn(p, deleter())
    {
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // shared_array will release p by calling d(p)
    //

    template<typename D> shared_array(T * p, D d): px(p), pn(p, d)
    {
    }

//  generated copy constructor, assignment, destructor are fine

    void reset(T * p = 0)
    {
        BOOST_ASSERT(p == 0 || p != px);
        this_type(p).swap(*this);
    }

    template <typename D> void reset(T * p, D d)
    {
        this_type(p, d).swap(*this);
    }

    T & operator[] (std::ptrdiff_t i) const // never throws
    {
        BOOST_ASSERT(px != 0);
        BOOST_ASSERT(i >= 0);
        return px[i];
    }
    
    T * get() const // never throws
    {
        return px;
    }

    bool unique() const // never throws
    {
        return pn.unique();
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    void swap(shared_array<T> & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

private:

    T * px;                     // contained pointer
    detail::shared_count pn;    // reference counter

};  // shared_array

template<typename T> inline bool operator==(shared_array<T> const & a, shared_array<T> const & b) // never throws
{
    return a.get() == b.get();
}

template<typename T> inline bool operator!=(shared_array<T> const & a, shared_array<T> const & b) // never throws
{
    return a.get() != b.get();
}

template<typename T> inline bool operator<(shared_array<T> const & a, shared_array<T> const & b) // never throws
{
    return std::less<T*>()(a.get(), b.get());
}

template<typename T> void swap(shared_array<T> & a, shared_array<T> & b) // never throws
{
    a.swap(b);
}

} // namespace boost

#endif  // #ifndef BOOST_MSVC6_MEMBER_TEMPLATES

#endif  // #ifndef BOOST_SHARED_ARRAY_HPP_INCLUDED
