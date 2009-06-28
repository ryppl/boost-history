// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_HPP
#define BOOST_CLONEABLE_HPP

#include <boost/abstract_allocator.hpp>
#include <boost/aligned_storage.hpp>

namespace boost 
{
	namespace cloneable 
	{
		/// common base for all base types for hierachies
		struct common_base
		{
			virtual ~common_base() { }

			virtual common_base *allocate(abstract_allocator &alloc) const = 0;
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

		template <class Derived>
		size_t base<Derived>::alignment = aligned_storage<sizeof(Derived)>::alignment;

		/// a cloning allocator
		struct allocator
		{
			template< class U >
			static U* allocate_clone( const U& r )
			{
				throw;
			}

			template< class U >
			static void deallocate_clone( const U* clone )
			{
				//throw;
			}

			// idea: pass allocator to the clone_allocator.
			// allocator rebind could be done in the ptr_container.
			// calling this must be disabled at compile-time for types that are not boost::is_convertible<cloneable::base<U> *, U*>
			template< class U, class Alloc >
			static U* allocate_clone( const U& r, Alloc &alloc )
			{
				U *ptr = r.copy_construct(r, alloc);
				return ptr;
			}

			// this is not even needed? 
			template< class U, class Alloc >
			static U* deallocate_clone( const U* r, Alloc &alloc )
			{
				typename Alloc::template rebind<U>::other my_alloc(alloc);
				my_alloc.deallocate(const_cast<U *>(r));
			}
		};

	} // namespace cloneable

} // namespace boost

#endif // BOOST_CLONEABLE_HPP

//EOF
