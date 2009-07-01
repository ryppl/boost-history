// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_COMMON_BASE_HPP
#define BOOST_CLONEABLE_COMMON_BASE_HPP

#include <string>
#include <boost/functional/hash_fwd.hpp>
#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/abstract_allocator.hpp>
#include <boost/cloneable/detail/make_clone_allocator.hpp>

namespace boost
{
	namespace cloneable
	{
		/// default base type used for object hierarchies to be stored in a given
		/// container or containers. the user can supply their own when using 
		/// cloneable<Derived, Base> this will be used by default.
		struct default_base_type
		{
			virtual ~default_base_type() { }
		};

		/// root structure for the cloneable object system
		template <class Base, class DefaultCtor>
		struct abstract_base : virtual Base
		{
			typedef Base base_type;
			typedef abstract_base<Base,DefaultCtor> this_type;

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

			/// for use with types that use multiple inheritance - select which sub-object to clone.
			/// can also be used when there is just one cloneable sub-object to avoid having to
			/// dynamic cast the result.
			template <class Ty>
			Ty *clone_as(abstract_allocator &alloc) const
			{
				const base<Ty, Base, DefaultCtor> *ptr = dynamic_cast<const base<Ty, Base,DefaultCtor> *>(this);
				if (ptr == 0)
					throw std::bad_cast();
				this_type *cloned = ptr->clone(alloc);
				return dynamic_cast<Ty *>(cloned);
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

			/// make a copy of this instance using default allocator, 
			/// selecting sub-object to clone
			template <class Ty>
			Ty *clone_as() const
			{
				make_clone_allocator<default_allocator>::type alloc;
				return clone_as<Ty>(alloc);
			}

			/// overridable to-string function, for utility
			virtual std::string to_string() const { return "cloneable"; }

			/// overridable hash function, for utility
			virtual size_t hash_value() const { return 0; }

			/// return a hash value for the object
			size_t hash() const 
			{ 
				if (size_t value = hash_value())
					return value;
				return 0;//ptrdiff_t(reinterpret_cast<const char *>(this) - 0);
			}

		};

	} // namespace cloneable

	template <class B>
	struct hash<cloneable::abstract_base<B> >
	{
		size_t operator()(cloneable::abstract_base<B> const &base) const
		{
			return base.hash();
		}
	};

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_COMMON_BASE_HPP

//EOF
