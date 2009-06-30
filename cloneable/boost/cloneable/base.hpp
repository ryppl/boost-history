// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_BASE_HPP
#define BOOST_CLONEABLE_BASE_HPP

#include <boost/aligned_storage.hpp>
#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/abstract_base.hpp>

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
			struct create_new<Derived, no_default_construction>
			{
				template <class Ty, class Alloc>
				static Derived *given(Ty *self, Alloc &alloc, size_t alignment)
				{
					throw no_default_construction();
				}
			};
		}

		/// base for the given derived type, using the given base class
		template <class Derived, class Base, class HasDefaultCtor>
		struct base : abstract_base<Base,HasDefaultCtor>
		{
			typedef Derived derived_type;
			typedef Base base_type;
			typedef abstract_base<Base,HasDefaultCtor> abstract_base_type;
			typedef base<Derived, Base,HasDefaultCtor> this_type;

			static const size_t alignment;		///< required alignment for allocation
			mutable derived_type *self_ptr;		///< pointer to derived object in this

		public:
			base() 
			{ 
				self_ptr = static_cast<Derived *>(this); 
			}

			virtual this_type *allocate(abstract_allocator &alloc) const 
			{
				abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(derived_type), alignment);
				Derived *ptr = reinterpret_cast<Derived *>(bytes);
				ptr->this_type::self_ptr = ptr;
				return ptr;
			}

			void deallocate(abstract_allocator &alloc)
			{
				Derived *ptr = dynamic_cast<Derived *>(this);
				alloc.deallocate_bytes(reinterpret_cast<abstract_allocator::pointer>(ptr), alignment);
			}

			virtual this_type *create_new(abstract_allocator &alloc) const 
			{
				return detail::create_new<Derived,HasDefaultCtor>::given(this, alloc, alignment);
			}

			virtual this_type *copy_construct(abstract_allocator &alloc) const 
			{ 
				abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(derived_type), alignment);
				Derived *ptr = reinterpret_cast<Derived *>(bytes);
				ptr->this_type::self_ptr = ptr;
				new (ptr->this_type::self_ptr) Derived(static_cast<const Derived &>(*this));
				return ptr;
			}
		};

		/// ensure correct alignment when allocating derived instances
		template <class Derived, class Base, class HasDefaultCtor>
		const size_t base<Derived, Base, HasDefaultCtor>::alignment = aligned_storage<sizeof(Derived)>::alignment;

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_BASE_HPP

//EOF
