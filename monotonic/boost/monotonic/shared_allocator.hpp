// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_SHARED_ALLOCATOR_H
#define BOOST_MONOTONIC_SHARED_ALLOCATOR_H

#include <boost/monotonic/allocator_base.hpp>

namespace boost
{
	namespace monotonic
	{
		template <class>
		struct shared_allocator;

		template <> 
		struct shared_allocator<void>
		{
			typedef void* pointer;
			typedef const void* const_pointer;

			typedef void value_type;
			template <class U> 
			struct rebind 
			{ 
				typedef shared_allocator<U> other; 
			};
		};

		template <class T> 
		struct shared_allocator : allocator_base<T, shared_allocator<T> >
		{
			typedef allocator_base<T, shared_allocator<T> > Parent;
			using typename Parent::size_type;
			using typename Parent::difference_type;
			using typename Parent::pointer;
			using typename Parent::const_pointer;
			using typename Parent::reference;
			using typename Parent::const_reference;
			using typename Parent::value_type;

			template <class U> 
			struct rebind 
			{ 
				typedef shared_allocator<U> other; 
			};

			shared_allocator() throw() 
				: Parent(static_shared_storage)	{ }

			shared_allocator(shared_storage_base &store) throw() 
				: Parent(store) { }

			shared_allocator(const shared_allocator& alloc) throw() 
				: Parent(alloc) { }

			template <class U> 
			shared_allocator(const shared_allocator<U> &alloc) throw()
				: Parent(alloc) { }

			friend bool operator==(shared_allocator<T> const &A, shared_allocator<T> const &B) 
			{ 
				return static_cast<Parent const &>(A) == static_cast<Parent const &>(B);
			}

			friend bool operator!=(shared_allocator<T> const &A, shared_allocator<T> const &B) 
			{ 
				return static_cast<Parent const &>(A) == static_cast<Parent const &>(B);
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_ALLOCATOR_H

//EOF
