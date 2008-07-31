/**
	@file
	Boost shifted_allocator.hpp header file.

	@note
	Copyright (c) 2008 Phil Bouchard <phil@fornux.com>.

	Distributed under the Boost Software License, Version 1.0.

	See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt

	See http://www.boost.org/libs/smart_ptr/doc/index.html for documentation.
*/


#ifndef BOOST_SHIFTED_ALLOCATOR_HPP_INCLUDED
#define BOOST_SHIFTED_ALLOCATOR_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/shifted_ptr.hpp>


namespace boost
{

namespace detail
{

namespace sh
{


/**
    STL compliant allocator.
*/

template <typename T>
    class shifted_allocator
    {
    public:
        typedef size_t                  size_type;
        typedef ptrdiff_t               difference_type;
        typedef shifted_ptr<T>          pointer;
        typedef const shifted_ptr<T>    const_pointer;
        typedef T &                     reference;
        typedef const T &               const_reference;
        typedef T                       value_type;

        template <typename U>
            struct rebind
            {
                typedef shifted_allocator<U> other;
            };

        shifted_allocator() throw()                                 {}
        shifted_allocator(const shifted_allocator &) throw()        {}
        template <typename U>
            shifted_allocator(const shifted_allocator<U> &) throw() {}

        ~shifted_allocator() throw()                                {}
        pointer address(reference x) const                          { return & x; }
        const_pointer address(const_reference x) const              { return & x; }

        value_type * allocate(size_type s, const void * = 0)
        {
            shifted<T> * p = (shifted<T> *) shifted<T>::operator new(sizeof(shifted<T>));

            return static_cast<shifted<T> *>(::new (p) owned_base)->element();
        }

        void deallocate(value_type * p, size_type)
        {
        }

        size_type max_size() const throw()
        {
            return size_t(-1) / sizeof(T);
        }

        void construct(value_type * p, const value_type & x)
        {
            ::new (p) T(x);
        }

        void destroy(value_type * p)
        {
        }
    };

template <typename T>
    inline bool operator == (const shifted_allocator<T> &, const shifted_allocator<T> &)
    {
        return true;
    }

template <typename T>
    inline bool operator != (const shifted_allocator<T> &, const shifted_allocator<T> &)
    {
        return false;
    }


} // namespace sh

} // namespace detail

using detail::sh::shifted_allocator;
using detail::sh::operator ==;
using detail::sh::operator !=;

} // namespace boost

#endif  // #ifndef BOOST_SHIFTED_ALLOCATOR_HPP_INCLUDED
