// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_DETAIL_POINTER_HPP
#define BOOST_CLONEABLE_DETAIL_POINTER_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/base.hpp>
#include <boost/cloneable/allocator.hpp>

namespace boost
{
	namespace cloneable
	{
		/// a pointer store that can retrieve pointers from up and down 
		/// the inheritance tree. stores the allocator that made it.
		template <class U, class B, class Alloc>
		struct instance
		{
			typedef U derived_type;
			typedef B base_type;
			typedef Alloc allocator_type;
			typedef base<derived_type, base_type> cloneable_type;
			typedef typename cloneable_type::abstract_base_type abstract_base_type;

		private:
			cloneable_type *ptr;
			allocator_type *alloc;

		public:
			instance() : ptr(0), alloc(0) { }

			instance(allocator_type &al) : ptr(0), alloc(&al)
			{
				allocate();
			}
			template <class A0>
			instance(allocator_type &al, A0 a0) : ptr(0), alloc(&al)
			{
				allocate();
				new (to_derived()) U(a0);
			}
			template <class A0, class A1>
			instance(allocator_type &al, A0 a0, A1 a1) : ptr(0), alloc(&al)
			{
				allocate();
				new (to_derived()) U(a0, a1);
			}
			template <class A0, class A1, class A2>
			instance(allocator_type &al, A0 a0, A1 a1, A2 a2) : ptr(0), alloc(&al)
			{
				allocate();
				new (to_derived()) U(a0, a1);
			}

			void allocate()
			{
				if (!alloc)
					return;
				ptr = cloneable::allocate<U>(*alloc);
			}
			void release()
			{
				if (!alloc)
					return;
				cloneable::release(ptr, *alloc);
				ptr = 0;
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
				if (!ptr)
					return 0;
				return ptr->cloneable_type::self_ptr;
			}
		};

	} // namespace cloneable

} // namespace boost

#endif // BOOST_CLONEABLE_DETAIL_POINTER_HPP

//EOF
