// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_DETAIL_CONTAINER_BASE_HPP
#define BOOST_HETEROGENOUS_DETAIL_CONTAINER_BASE_HPP

#include <boost/heterogenous/detail/prefix.hpp>

namespace boost
{
	namespace heterogenous
	{
		namespace detail
		{
			template <class U, class Alloc>
			U *allocate_type(Alloc &al)
			{
				typename Alloc::template rebind<U>::other alloc(al);
				return alloc.allocate(1);
			}

			// TODO: use variadic template arguments, or BOOST_PP

			template <class U, class Alloc>
			U *construct_type(Alloc &al)
			{
				typename Alloc::template rebind<U>::other alloc(al);
				U *ptr = alloc.allocate(1);
				alloc.construct(ptr);
				return ptr;
			}

			template <class U, class Alloc, class A0>
			U *construct_type(Alloc &al, A0 a0)
			{
				U *ptr = allocate_type<U>(al);
				new (ptr) U(a0);
				return ptr;
			}

			template <class U, class Alloc, class A0, class A1>
			U *construct_type(Alloc &al, A0 a0, A1 a1)
			{
				U *ptr = allocate_type<U>(al);
				new (ptr) U(a0, a1);
				return ptr;
			}

			template <class U, class Alloc, class A0, class A1, class A2>
			U *construct_type(Alloc &al, A0 a0, A1 a1, A2 a2)
			{
				U *ptr = allocate_type<U>(al);
				new (ptr) U(a0, a1, a2);
				return ptr;
			}

			// etc...

		} // namespace detail

	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_DETAIL_CONTAINER_BASE_HPP

//EOF

