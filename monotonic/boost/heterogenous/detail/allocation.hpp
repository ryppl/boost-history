// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_DETAIL_CONTAINER_BASE_HPP
#define BOOST_HETEROGENOUS_DETAIL_CONTAINER_BASE_HPP

#include <boost/heterogenous/detail/prefix.hpp>
#include <boost/heterogenous/cloneable.hpp>

namespace boost
{
	namespace heterogenous
	{
		namespace detail
		{
			template <class U, class B>
			struct pointer
			{
				typedef U derived_type;
				typedef B base_type;
				typedef cloneable<derived_type, base_type> ptr_type;
				typedef typename cloneable<derived_type, base_type>::this_type cloneable_type;
				typedef typename cloneable_type::abstract_base_type abstract_base_type;

			private:
				ptr_type *ptr;

			public:
				pointer(U *p = 0) : ptr(dynamic_cast<cloneable_type *>(p))
				{
				}
				abstract_base_type *to_abstract() const
				{
					return ptr;
				}
				base_type *to_base() const
				{
					return ptr;
				}
				cloneable_type *to_cloneable() const
				{
					return ptr;
				}
				derived_type *to_derived() const
				{
					return ptr->cloneable_type::self_ptr;
				}
			};

			template <class U, class Alloc>
			U *allocate_type(Alloc &al)
			{
				typename Alloc::template rebind<U>::other alloc(al);
				return alloc.allocate(1);
			}

			// TODO: use variadic template arguments, or BOOST_PP

			template <class U, class Base, class Alloc>
			pointer<U,Base> construct_type(Alloc &al)
			{
				typename Alloc::template rebind<U>::other alloc(al);
				U *ptr = alloc.allocate(1);
				alloc.construct(ptr);
				return ptr;
			}


			template <class U, class Base, class Alloc, class A0>
			pointer<U,Base> construct_type(Alloc &al, A0 a0)
			{
				U *ptr = allocate_type<U>(al);
				new (ptr) U(a0);
				return ptr;
			}

			template <class U, class Base, class Alloc, class A0, class A1>
			pointer<U,Base> construct_type(Alloc &al, A0 a0, A1 a1)
			{
				U *ptr = allocate_type<U>(al);
				new (ptr) U(a0, a1);
				return ptr;
			}

			template <class U, class Base, class Alloc, class A0, class A1, class A2>
			pointer<U,Base> construct_type(Alloc &al, A0 a0, A1 a1, A2 a2)
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

