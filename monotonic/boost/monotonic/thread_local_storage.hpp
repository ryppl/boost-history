// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_THREAD_LOCAL_STORAGE_HPP
#define BOOST_MONOTONIC_THREAD_LOCAL_STORAGE_HPP

#include <boost/monotonic/storage.hpp>
#include <boost/monotonic/static_storage.hpp>
#include <boost/thread.hpp>

namespace boost
{
	namespace monotonic
	{
		/// thread-local storage
		template <size_t InlineSize, size_t MinHeapSize, class Al>
		struct thread_local_storage : shared_storage_base
		{
			typedef storage<InlineSize, MinHeapSize, Al> AllocatorStorage;
			typedef boost::thread_specific_ptr<AllocatorStorage> Storage;
			typedef thread_local_storage<InlineSize, MinHeapSize, Al> This;

		private:
			Storage store;
			static void no_delete(Storage *) { }

		public:
			thread_local_storage() : store(&This::no_delete)
			{
				store.reset(&static_thread_local_storage);
			}
			size_t used() const
			{
				return store->used();
			}
			void reset()
			{
				store->reset();
			}
			void release()
			{
				store->release();
			}
			void *allocate(size_t num_bytes, size_t alignment)
			{
				return store->allocate(num_bytes, alignment);
			}
			size_t remaining() const
			{
				return store->remaining();
			}
			size_t fixed_remaining() const
			{
				return store->fixed_remaining();
			}
			size_t max_size() const
			{
				return store->max_size();
			}
		};

		extern static_storage_base<
			DefaultSizes::StaticInlineSize
			, DefaultSizes::StaticMinHeapIncrement
			, std::allocator<char>
			, thread_local_storage> 
			static_thread_local_storage;

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_THREAD_LOCAL_STORAGE_HPP

//EOF

