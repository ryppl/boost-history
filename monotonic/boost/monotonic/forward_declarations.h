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
#include <boost/monotonic/config.h>

namespace boost
{
	namespace monotonic
	{
		/// storage for an allocator that is on the stack or heap
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

		/// a monotonic allocator has a storage buffer and a no-op deallocate() method
		template <class> 
		struct allocator;
	}
}

#endif // BOOST_MONOTONIC_FORWARD_DECLARATIONS_H

//EOF
