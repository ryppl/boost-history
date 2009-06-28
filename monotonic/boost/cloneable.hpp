// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_HPP
#define BOOST_CLONEABLE_HPP

#include <boost/abstract_allocator.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace boost 
{
	namespace cloneable 
	{
		/// common base for all base types for hierachies
		struct common_base
		{
			virtual ~common_base() { }

			virtual common_base *allocate(abstract_allocator &alloc) const = 0;
			virtual void deallocate(common_base *, abstract_allocator &alloc) const = 0;
			virtual common_base *create(abstract_allocator &alloc) const = 0;
			virtual common_base *copy_construct(const common_base &original, abstract_allocator &alloc) const = 0;
		};

		/// base of the given derived type
		template <class Derived>
		struct base : common_base
		{
			typedef Derived derived_type;
			typedef base<derived_type> this_type;

		private:
			static size_t alignment;
			mutable derived_type *self_ptr;

			derived_type *&self(derived_type *ptr) const
			{
				return ptr->this_type::self_ptr;
			}

		public:
			base() : self_ptr(0) { }

			virtual base<Derived> *allocate(boost::abstract_allocator &alloc) const 
			{
				boost::abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(derived_type), alignment);
				Derived *ptr = reinterpret_cast<Derived *>(bytes);
				self(ptr) = ptr;
				return ptr;
			}

			void deallocate(common_base *object, abstract_allocator &alloc) const
			{
				alloc.deallocate_bytes(reinterpret_cast<abstract_allocator::pointer>(object));
			}

			virtual base<Derived> *create(boost::abstract_allocator &alloc) const 
			{
				base<Derived> *ptr = allocate(alloc);
				new (ptr->self_ptr) Derived();
				return ptr;
			}

			virtual base<Derived> *copy_construct(const common_base &original, boost::abstract_allocator &alloc) const 
			{ 
				base<Derived> *ptr = allocate(alloc);
				new (ptr->self_ptr) Derived(static_cast<const Derived &>(original));
				return ptr;
			}
		};

		/// ensure correct alignment when allocating derived instances
		template <class Derived>
		size_t base<Derived>::alignment = aligned_storage<sizeof(Derived)>::alignment;

		/// a cloning allocator
		struct allocator
		{
			template <class Base>
			static Base* allocate_clone( const Base& object )
			{
				throw;
			}

			template <class Base>
			static void deallocate_clone( const Base* clone )
			{
				throw;
			}

			template <class Base, class Alloc>
			static Base* allocate_clone(const Base& object, Alloc &alloc )
			{
				// calling copy_construct must be disabled at compile-time for types that are not boost::is_convertible<cloneable::base<U> *, U*>
				return object.copy_construct(object, alloc);
			}

			template <class Base, class Alloc>
			static void deallocate_clone(const Base *object, Alloc &alloc )
			{
				object->deallocate(const_cast<Base *>(object), alloc);
			}
		};

		namespace impl
		{
			template <class Alloc>
			struct cloneable_allocator : Alloc, boost::abstract_allocator
			{
				typedef typename Alloc::template rebind<char>::other CharAlloc;

				boost::abstract_allocator::pointer allocate_bytes(size_t num_bytes, size_t alignment)
				{
					CharAlloc alloc;
					// todo: alignment; this is done already for monotonic, copy that here
					return alloc.allocate(num_bytes);
				}

				void deallocate_bytes(boost::abstract_allocator::pointer ptr)
				{
					CharAlloc alloc;
					alloc.deallocate(ptr, 1);
				}
			};

			template <class Alloc, bool>
			struct make_cloneable_allocator
			{		
				typedef cloneable_allocator<Alloc> type;
			};

			template <class Alloc>
			struct make_cloneable_allocator<Alloc, true>
			{
				typedef Alloc type;
			};
		}

		template <class Alloc>
		struct make_cloneable_allocator
		{
			typedef boost::is_convertible<Alloc *, boost::abstract_allocator *> is_convertible;
			BOOST_STATIC_CONSTANT(bool, is_cloneable = is_convertible::value);
			typedef typename impl::make_cloneable_allocator<Alloc, is_cloneable>::type type;
		};

	} // namespace cloneable

} // namespace boost

#endif // BOOST_CLONEABLE_HPP

//EOF
