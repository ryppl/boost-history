// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_SHARED_STORAGE_H
#define BOOST_MONOTONIC_SHARED_STORAGE_H

#include <boost/monotonic/storage.hpp>
#include <boost/monotonic/static_storage.hpp>
#include <boost/thread/mutex.hpp>

namespace boost
{
	namespace monotonic
	{
		/// thread-safe storage
		template <size_t InlineSize, size_t MinHeapSize, class Al>
		struct shared_storage : shared_storage_base
		{
			typedef storage<InlineSize, MinHeapSize, Al> Storage;

		private:
			Storage storage;
			mutable mutex guard;

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
				storage.reset();
			}
			void release()
			{
				mutex::scoped_lock lock(guard);
				storage.release();
			}
			void *allocate(size_t num_bytes, size_t alignment)
			{
				mutex::scoped_lock lock(guard);
				return storage.allocate(num_bytes, alignment);
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

		extern static_storage_base<
			DefaultSizes::StaticInlineSize
			, DefaultSizes::StaticMinHeapIncrement
			, std::allocator<char>
			, shared_storage> 
		static_shared_storage;
		
	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_SHARED_STORAGE_H

//EOF

