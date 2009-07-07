// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_DETAIL_ALLOCATION_HPP
#define BOOST_CLONEABLE_DETAIL_ALLOCATION_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/base.hpp>
#include <boost/cloneable/instance.hpp>

namespace boost
{
    namespace cloneable
    {
        /*
        namespace detail
        {
            // TODO: use variadic template arguments, or BOOST_PP

            template <class U, class Base, class Alloc>
            pointer<U,Base,Alloc> construct(Alloc &al)
            {
                pointer<U,Base,Alloc> ptr(al);
                ptr.construct();
                return ptr;
            }

            template <class U, class Base, class Alloc, class A0>
            pointer<U,Base,Alloc> construct(Alloc &al, A0 a0)
            {
                pointer<U,Base,Alloc> ptr(al);
                new (ptr.to_derived()) U(a0);
                return ptr;
            }

            template <class U, class Base, class Alloc, class A0, class A1>
            pointer<U,Base,Alloc> construct(Alloc &al, A0 a0, A1 a1)
            {
                pointer<U,Base,Alloc> ptr(al);
                new (ptr.to_derived()) U(a0, a1);
                return ptr;
            }

            template <class U, class Base, class Alloc, class A0, class A1, class A2>
            pointer<U,Base,Alloc> construct(Alloc &al, A0 a0, A1 a1, A2 a2)
            {
                pointer<U,Base,Alloc> ptr(al);
                new (ptr.to_derived()) U(a0, a1, a2);
                return ptr;
            }

            // etc...

        } // namespace detail
        */

    } // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_DETAIL_ALLOCATION_HPP

//EOF

