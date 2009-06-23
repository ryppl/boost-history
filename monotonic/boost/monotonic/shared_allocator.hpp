// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_SHARED_ALLOCATOR_HPP
#define BOOST_MONOTONIC_SHARED_ALLOCATOR_HPP

#include <boost/monotonic/allocator_base.hpp>

namespace boost
{
	namespace monotonic
	{
		template <class Region> 
		struct shared_allocator<void, Region>
		{
			typedef void* pointer;
			typedef const void* const_pointer;

			typedef void value_type;
			template <class U> 
			struct rebind 
			{ 
				typedef shared_allocator<U, Region> other; 
			};
		};

		template <class T, class Region> 
		struct shared_allocator : allocator_base<T, shared_allocator<T, Region> >
		{
			typedef allocator_base<T, shared_allocator<T, Region> > Parent;
			using typename Parent::size_type;
			using typename Parent::difference_type;
			using typename Parent::pointer;
			using typename Parent::const_pointer;
			using typename Parent::reference;
			using typename Parent::const_reference;
			using typename Parent::value_type;

			//typedef storage<DefaultSizes::InlineSize, DefaultSizes::MinHeapIncrement, std::allocator<char>, shared_storage> 
			//	StorageType;

			template <class U> 
			struct rebind 
			{ 
				typedef shared_allocator<U, Region> other; 
			};

			shared_allocator() throw() 
				: Parent(get_region_storage<Region>())	{ }

			shared_allocator(const shared_allocator& alloc) throw() 
				: Parent(alloc) { }

			template <class U> 
			shared_allocator(const shared_allocator<U, Region> &alloc) throw()
				: Parent(alloc) { }

			friend bool operator==(shared_allocator<T,Region> const &A, shared_allocator<T,Region> const &B) 
			{ 
				return static_cast<Parent const &>(A) == static_cast<Parent const &>(B);
			}

			friend bool operator!=(shared_allocator<T,Region> const &A, shared_allocator<T,Region> const &B) 
			{ 
				return static_cast<Parent const &>(A) == static_cast<Parent const &>(B);
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_SHARED_ALLOCATOR_HPP

//EOF
