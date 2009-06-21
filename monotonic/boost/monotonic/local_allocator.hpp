// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_SHARED_ALLOCATOR_H
#define BOOST_MONOTONIC_SHARED_ALLOCATOR_H

#include <boost/monotonic/allocator_base.hpp>
#include <boost/monotonic/thread_local_storage.hpp>

namespace boost
{
	namespace monotonic
	{
		template <> 
		struct local_allocator<void>
		{
			typedef void* pointer;
			typedef const void* const_pointer;

			typedef void value_type;
			template <class U> 
			struct rebind 
			{ 
				typedef local_allocator<U> other; 
			};
		};

		template <class T> 
		struct local_allocator : allocator_base<T, local_allocator<T> >
		{
			typedef allocator_base<T, local_allocator<T> > Parent;
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
				typedef local_allocator<U> other; 
			};

			local_allocator() throw() 
				: Parent(static_thread_local_storage)	{ }

			local_allocator(const local_allocator& alloc) throw() 
				: Parent(alloc) { }

			template <class U> 
			local_allocator(const local_allocator<U> &alloc) throw()
				: Parent(alloc) { }

			friend bool operator==(local_allocator<T> const &A, local_allocator<T> const &B) 
			{ 
				return static_cast<Parent const &>(A) == static_cast<Parent const &>(B);
			}

			friend bool operator!=(local_allocator<T> const &A, local_allocator<T> const &B) 
			{ 
				return static_cast<Parent const &>(A) == static_cast<Parent const &>(B);
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_ALLOCATOR_H

//EOF
