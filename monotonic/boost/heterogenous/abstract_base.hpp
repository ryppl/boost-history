// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_COMMON_BASE_HPP
#define BOOST_HETEROGENOUS_COMMON_BASE_HPP

#include <boost/heterogenous/detail/prefix.hpp>
#include <boost/heterogenous/abstract_allocator.hpp>

namespace boost
{
	namespace heterogenous
	{
		struct default_base_type
		{
			virtual ~default_base_type() { }
		};

		/// common base for all base types for hierachies
		template <class Base = default_base_type>
		struct abstract_base : Base
		{
			typedef Base base_type;
			typedef abstract_base<Base> this_type;

			//virtual base_type *allocate(abstract_allocator &alloc) const /*= 0;*/ { return 0; }
			//virtual void deallocate(base_type *, abstract_allocator &alloc) const /*= 0;*/ { }
			//virtual base_type *create(abstract_allocator &alloc) const /*= 0;*/ { return 0; }
			//virtual base_type *copy_construct(const base_type &original, abstract_allocator &alloc) const /*= 0;*/ { return 0; }

			virtual this_type *allocate(abstract_allocator &alloc) const = 0;
			virtual void deallocate(base_type &, abstract_allocator &alloc) const = 0;

			virtual this_type *create_new(abstract_allocator &alloc) const = 0;
			virtual this_type *copy_construct(const base_type &original, abstract_allocator &alloc) const = 0;
		};

	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_COMMON_BASE_HPP

//EOF
