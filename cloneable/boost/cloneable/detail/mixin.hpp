// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_IS_DERIVED_HPP
#define BOOST_CLONEABLE_IS_DERIVED_HPP

#include <boost/aligned_storage.hpp>
#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/abstract_base.hpp>

namespace boost
{
	namespace cloneable
	{
		namespace detail
		{
			// mixin structure between base<> and abstract_base<> that supplies
			// an interface to allow casting, cloning and creation of sub-objects
			// of different types and default-constructableness
			template <class Derived, class Base>
			struct is_derived 
				: abstract_base<Base> 
			{
				typedef Derived derived_type;
				typedef Base base_type;
				typedef abstract_base<base_type> abstract_base_type;
				typedef is_derived<derived_type, base_type> this_type;

				mutable Derived *self_ptr;		///< pointer to derived object in this
				static const size_t alignment;		///< required alignment for allocation

				is_derived()
				{
					self_ptr = static_cast<derived_type *>(this); 
				}

				const std::type_info &get_type() const
				{
					return typeid(derived_type);
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
					Derived *ptr = self_ptr;
					alloc.deallocate_bytes(reinterpret_cast<abstract_allocator::pointer>(ptr), alignment);
				}

				virtual this_type *copy_construct(abstract_allocator &alloc) const 
				{ 
					abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(derived_type), alignment);
					Derived *ptr = reinterpret_cast<Derived *>(bytes);
					ptr->this_type::self_ptr = ptr;
					new (ptr->this_type::self_ptr) Derived(static_cast<const Derived &>(*this));
					return ptr;
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

				template <class Ty>
				Ty *create_as(abstract_allocator &alloc) const
				{
					typedef is_derived<Ty, Base> Embedded;
					const Embedded *cross_cast = dynamic_cast<const Embedded *>(this);
					if (cross_cast == 0)
						throw std::bad_cast();
					abstract_base_type *base = cross_cast->create_new(alloc);
					return static_cast<Ty *>(static_cast<Embedded *>(base));
				}

				template <class Ty>
				Ty *create_as() const
				{
					make_clone_allocator<default_allocator>::type alloc;
					return create_as<Ty>(alloc);
				}
			};

			/// ensure correct alignment when allocating derived instances
			template <class Derived, class Base>
			const size_t is_derived<Derived, Base>::alignment = aligned_storage<sizeof(Derived)>::alignment;

		} // namespace detail

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_IS_DERIVED_HPP

//EOF
