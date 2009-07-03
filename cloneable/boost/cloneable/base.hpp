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
				template <class Self, class Alloc>
				static Derived *given(Self *self, Alloc &alloc, size_t alignment)
				{
					throw no_default_construction();
				}
			};

		} // namespace impl

		template <class Derived, class Base>
		struct is_derived 
			: abstract_base<Base> 
		{
			typedef Derived derived_type;
			typedef Base base_type;
			typedef abstract_base<base_type> abstract_base_type;
			typedef is_derived<derived_type, base_type> this_type;
			using abstract_base_type::clone;

			mutable Derived *self_ptr;		///< pointer to derived object in this

			is_derived()
			{
				self_ptr = static_cast<derived_type *>(this); 
			}

			/// for use with types that use multiple inheritance - select which sub-object to clone.
			/// can also be used when there is just one cloneable sub-object to avoid having to
			/// dynamic cast the result.
			template <class Ty>
			Ty *clone_as(abstract_allocator &alloc) const
			{
				const is_derived<Ty,Base> *ptr = dynamic_cast<const is_derived<Ty,Base> *>(this);
				if (ptr == 0)
					throw std::bad_cast();
				abstract_base_type *cloned = ptr->clone(alloc);
				return dynamic_cast<Ty *>(cloned);
			}

			/// make a copy of this instance using default allocator, 
			/// selecting sub-object to clone
			template <class Ty>
			Ty *clone_as() const
			{
				make_clone_allocator<default_allocator>::type alloc;
				return clone_as<Ty>(alloc);
			}
		};

		/// base for the given derived type, using the given base class and default construction tag
		template <class Derived, class Base, class DefaultCtorTag>
		struct base 
			: is_derived<Derived, Base>
			, virtual is_cloneable_tag
			, virtual DefaultCtorTag
		{
			typedef Derived derived_type;
			typedef Base base_type;
			typedef DefaultCtorTag default_constructable_type;

			typedef base<derived_type, base_type, default_constructable_type> this_type;
			typedef is_derived<Derived,Base> is_derived_type;

			static const size_t alignment;		///< required alignment for allocation

		public:
			base() 
			{ 
			}

			const std::type_info &get_type() const
			{
				return typeid(derived_type);
			}

			virtual this_type *allocate(abstract_allocator &alloc) const 
			{
				abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(derived_type), alignment);
				Derived *ptr = reinterpret_cast<Derived *>(bytes);
				ptr->is_derived_type::self_ptr = ptr;
				return ptr;
			}

			void deallocate(abstract_allocator &alloc)
			{
				Derived *ptr = is_derived_type::self_ptr;
				alloc.deallocate_bytes(reinterpret_cast<abstract_allocator::pointer>(ptr), alignment);
			}

			virtual this_type *create_new(abstract_allocator &alloc) const 
			{
				return detail::create_new<Derived, DefaultCtorTag>::given(this, alloc, alignment);
			}

			virtual this_type *copy_construct(abstract_allocator &alloc) const 
			{ 
				abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(derived_type), alignment);
				Derived *ptr = reinterpret_cast<Derived *>(bytes);
				ptr->is_derived_type::self_ptr = ptr;
				new (ptr->is_derived_type::self_ptr) Derived(static_cast<const Derived &>(*this));
				return ptr;
			}


		};

		/// ensure correct alignment when allocating derived instances
		template <class Derived, class Base, class DefaultCtor>
		const size_t base<Derived, Base, DefaultCtor>::alignment = aligned_storage<sizeof(Derived)>::alignment;

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_BASE_HPP

//EOF
