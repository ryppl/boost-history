// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/monotonic/storage.h>
//#include <boost/monotonic/shared_storage.h>

namespace boost
{
	namespace monotonic
	{
		/// a globally available storage buffer
		template <size_t InlineSize = 64*1024
			, size_t MinHeapIncrement = 2*1024*1024
			, class Al = std::allocator<char>
			, template <size_t, size_t, class> class Storage = storage 
		>
		struct static_storage_base;

		template <size_t InlineSize, size_t MinHeapIncrement, class Al, template <size_t, size_t, class> class Storage>
		struct static_storage_base : storage_base
		{
			typedef Al HeapAllocator;
			typedef Storage<InlineSize, MinHeapIncrement, HeapAllocator> StorageType;

		private:
			StorageType global;

		public:
			static_storage_base()
			{
			}
			static_storage_base(StorageType const &store) 
				: global(store)
			{
			}
			void reset()
			{
				global.reset();
			}
			void release()
			{
				global.release();
			}
			void *allocate(size_t num_bytes, size_t alignment)
			{
				return global.allocate(num_bytes, alignment);
			}
			size_t max_size() const
			{
				return global.max_size();
			}
			size_t used() const
			{
				return global.used();
			}
			size_t remaining() const
			{
				return global.remaining();
			}
		};

		/// 'static_storage' will be used by a default-constructed monotonic::allocator
		extern static_storage_base<> static_storage;

		//extern static_storage_base<8*1024, 8*1024*1024, std::allocator<char>, shared_storage> static_shared_storage;
	}
}

//EOF
