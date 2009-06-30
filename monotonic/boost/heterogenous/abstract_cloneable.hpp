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
		/// default base type used for object hierarchies to be stored in a given
		/// container or containers. the user can supply their own when using 
		/// cloneable<Derived, Base> this will be used by default.
		struct default_base_type
		{
			virtual ~default_base_type() { }
		};

		/// root structure for the heterogenous object system
		template <class Base>
		struct abstract_cloneable : Base
		{
			typedef Base base_type;
			typedef abstract_cloneable<Base> this_type;

			/// make storage for a new instance, but do not invoke any constructor
			virtual this_type *allocate(abstract_allocator &alloc) const = 0;

			/// free memory associated with the given instance
			virtual void deallocate(base_type &, abstract_allocator &alloc) const = 0;

			/// create a new object of the derived type
			virtual this_type *create_new(abstract_allocator &alloc) const = 0;

			/// create a clone using copy-constructor. this is implemented in cloneable<>, but can
			/// be overriden by the user in the derived type if required.
			virtual this_type *copy_construct(const base_type &original, abstract_allocator &alloc) const = 0;

			/// optional means to make a clone that does not use copy-construction.
			/// user can overload this in their derived type to provide custom clone implementation.
			virtual this_type *clone(const base_type &original, abstract_allocator &alloc) const { return 0; }

			/// make a copy of the given instance. try the custom clone method first, 
			/// then default to using the copy-constructor method
			this_type *make_copy(const base_type &original, abstract_allocator &alloc) const
			{
				if (this_type *copy = clone(original, alloc))
					return copy;
				return copy_construct(original, alloc);
			}
		};

	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_COMMON_BASE_HPP

//EOF
