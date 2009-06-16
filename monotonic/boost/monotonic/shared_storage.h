// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/monotonic/storage.h>
#include <boost/thread/mutex.hpp>

namespace boost
{
	namespace monotonic
	{
		/// thread-safe storage
		template <size_t InlineSize = 8*1024, size_t MinHeapSize = InlineSize*1000, class Al = std::allocator<char> >
		struct shared_storage : storage_base
		{
			typedef storage<InlineSize, MinHeapSize, Al> Storage;
		private:
			Storage storage;
			mutex guard;

		public:
			shared_storage()
			{
			}
			shared_storage(Al const &A)
				: storage(A)
			{
			}
			size_t used() const
			{
				mutex::scoped_lock lock(guard);
				return storage.used();
			}
			void reset()
			{
				mutex::scoped_lock lock(guard);
				return storage.reset();
			}
			void *allocate(size_t num_bytes, size_t alignment)
			{
				mutex::scoped_lock lock(guard);
				return storage.allocate(num_bytes, alignment);
			}
			void deallocate(void *, size_t)
			{
				// do nothing
			}
			size_t remaining() const
			{
				mutex::scoped_lock lock(guard);
				return storage.remaining();
			}
			size_t fixed_remaining() const
			{
				mutex::scoped_lock lock(guard);
				return storage.fixed_remaining();
			}
			size_t max_size() const
			{
				mutex::scoped_lock lock(guard);
				return storage.max_size();
			}

		};

	} // namespace monotonic

} // namespace boost

//EOF

