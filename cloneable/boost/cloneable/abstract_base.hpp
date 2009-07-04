// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_COMMON_BASE_HPP
#define BOOST_CLONEABLE_COMMON_BASE_HPP

#include <string>
#include <typeinfo>
#include <boost/functional/hash_fwd.hpp>
#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/abstract_allocator.hpp>
#include <boost/cloneable/detail/make_clone_allocator.hpp>

namespace boost
{
	namespace cloneable
	{
		/// root structure for the cloneable object system
		template <class Base>
		struct abstract_base 
			: virtual Base
			, virtual is_cloneable_tag
		{
			typedef Base base_type;
			typedef abstract_base<Base> this_type;

			virtual const std::type_info &get_type() const = 0;

			/// make storage for a new instance, but do not invoke any constructor
			virtual this_type *allocate(abstract_allocator &) const = 0;

			/// free memory associated with this instance. 
			/// this is effectively 'delete this', but without a call to the destructor
			virtual void deallocate(abstract_allocator &) = 0;

			/// create a new object of the derived type
			virtual this_type *create_new(abstract_allocator &) const = 0;

			/// create a clone using copy-constructor. this is implemented in cloneable<>, but can
			/// be overriden by the user in the derived type if required.
			virtual this_type *copy_construct(abstract_allocator &) const = 0;

			/// optional means to make a clone that does not use copy-construction.
			/// user can overload this in their derived type to provide custom clone implementation.
			virtual this_type *make_copy(abstract_allocator &) const { return 0; }

			/// make a copy of the given instance. try the custom clone method first, 
			/// then default to using the copy-constructor method
			this_type *clone(abstract_allocator &alloc) const
			{
				if (this_type *copy = make_copy(alloc))
					return copy;
				return copy_construct(alloc);
			}

			/// MUTABLE
			virtual this_type *make_copy(abstract_allocator &) { return 0; }

			/// MUTABLE 
			this_type *clone(abstract_allocator &alloc)
			{
				if (this_type *copy = make_copy(alloc))
					return copy;
				const this_type *const_this = const_cast<const this_type *>(this);
				if (this_type *copy = const_this->make_copy(alloc))
					return copy;
				return const_this->copy_construct(alloc);
			}

			/// non-virtual method that allocates using default allocator
			this_type *allocate() const
			{
				make_clone_allocator<default_allocator>::type alloc;
				return allocate(alloc);
			}

			/// non-virtual method that deallocates using default allocator
			void deallocate() const
			{
				make_clone_allocator<default_allocator>::type alloc;
				return deallocate(alloc);
			}

			/// non-virtual method that creates a new instance of derived type using default allocator
			this_type *create() const
			{
				make_clone_allocator<default_allocator>::type alloc;
				return create_new(alloc);
			}

			/// non-virtual method that creates a new instance of derived type using given allocator
			template <class Alloc>
			this_type *create(Alloc &alloc) const
			{
				return create_new(alloc);
			}

			/// non-virtual method that creates a new instance of derived type from this instance,
			/// using copy-constructor and default allocator
			this_type *copy_construct() const
			{
				make_clone_allocator<default_allocator>::type alloc;
				return copy_construct(alloc);
			}

			/// make a copy of this instance using default allocator
			this_type *clone() const
			{
				make_clone_allocator<default_allocator>::type alloc;
				return clone(alloc);
			}

			// these non-virtuals require mixin<D,B> to be defined before they can be 
			// implemented. See detail/mixin.hpp for their definitions
			template <class Derived>
			bool can_clone_as() const;

			template <class Derived>
			Derived *clone_as() const;

			template <class Derived>
			Derived *clone_as(abstract_allocator &) const;

			template <class Derived>
			bool can_create_as() const;

			template <class Derived>
			bool can_default_create_as() const;
			///

			/// overridable to-string function, for utility
			virtual std::string to_string() const { return "cloneable"; }

			/// return a hash value for the object
			/*
			size_t hash() const 
			{ 
				return base_type::hash_value();
			}
			*/

			friend bool operator==(const abstract_base &left, const abstract_base &right)
			{
				return static_cast<const base_type &>(left) == static_cast<const base_type &>(right);
			}
			friend bool operator<(const abstract_base &left, const abstract_base &right)
			{
				return static_cast<const base_type &>(left) < static_cast<const base_type &>(right);
			}
		};

	} // namespace cloneable

	/*
	template <class B>
	struct hash<cloneable::abstract_base<B> >
	{
		size_t operator()(cloneable::abstract_base<B> const &base) const
		{
			return base.hash();
		}
	};
	*/

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_COMMON_BASE_HPP

//EOF
