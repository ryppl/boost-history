// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_FORWARD_DECLARATIONS_H
#define BOOST_MONOTONIC_FORWARD_DECLARATIONS_H

#include <utility>
#include <limits>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/monotonic/config.hpp>

namespace boost
{
	namespace monotonic
	{
		/// fixed-sized storage for an allocator that is on the stack or heap
		template <size_t InlineSize = DefaultSizes::InlineSize>
		struct fixed_storage;

		/// storage that can span the stack/heap boundary.
		///
		/// allocation requests first use inline fixed_storage of InlineSize bytes.
		/// once that is exhausted, later requests are serviced from the heap.
		///
		/// all allocations remain valid until the storage goes out of scope.
		template <
			size_t InlineSize = DefaultSizes::InlineSize
			, size_t MinHeapIncrement = DefaultSizes::MinHeapIncrement
			, class Al = std::allocator<char> >
		struct storage;

		/// thread-safe storage
		template <
			size_t InlineSize = DefaultSizes::InlineSize
			, size_t MinHeapIncrement = DefaultSizes::MinHeapIncrement
			, class Al = std::allocator<char> >
		struct shared_storage;

		/// a globally available storage buffer
		template <size_t InlineSize = DefaultSizes::StaticInlineSize
			, size_t MinHeapIncrement = DefaultSizes::StaticMinHeapIncrement
			, class Al = std::allocator<char>
			, template <size_t, size_t, class> class Storage = storage 
		>
		struct static_storage_base;

		/// common to other monotonic allocators for type T of type Derived
		template <class T, class Derived>
		struct allocator_base;

		/// a monotonic allocator has a storage buffer and a no-op deallocate() method
		/// default to use static_storage_base<..., storage>
		template <class> 
		struct allocator;

		/// a monotonic shared_allocator has a shared storage buffer and a no-op deallocate() method
		/// defaults to use static_storage_base<..., shared_storage>
		template <class> 
		struct shared_allocator;
	
	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_FORWARD_DECLARATIONS_H

//EOF
