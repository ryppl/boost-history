// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_DETAIL_CREATE_NEW_HPP
#define BOOST_CLONEABLE_DETAIL_CREATE_NEW_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/allocator.hpp>

namespace boost
{
	namespace cloneable
	{
		namespace detail
		{
			template <class Derived, class HasDefaultCtor>
			struct create_new
			{
				template <class Self, class Alloc>
				static Derived *given(Self *self, Alloc &alloc, size_t alignment)
				{
					abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(Derived), alignment);
					Derived *ptr = reinterpret_cast<Derived *>(bytes);
					ptr->Self::self_ptr = ptr;
					new (ptr->Self::self_ptr) Derived();
					return ptr;
				}
			};
			template <class Derived>
			struct create_new<Derived, no_default_construction_tag>
			{
				template <class Self, class Alloc>
				static Derived *given(Self *self, Alloc &alloc, size_t alignment)
				{
					throw no_default_construction();
				}
			};
			
			// TODO: deal with conversion from bases with unknown_construction_tag

			template <class Derived>
			struct create_new<Derived, unknown_construction_tag>
			{
				template <class Self, class Alloc>
				static Derived *given(Self *self, Alloc &alloc, size_t alignment)
				{
					abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(Derived), alignment);
					Derived *ptr = reinterpret_cast<Derived *>(bytes);
					ptr->Self::self_ptr = ptr;
					new (ptr->Self::self_ptr) Derived(Derived());
					return ptr;
				}
			};
		} // namespace detail

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_DETAIL_CREATE_NEW_HPP

//EOF
