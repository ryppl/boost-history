// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_STATIC_STORAGE_BASE_H
#define BOOST_MONOTONIC_STATIC_STORAGE_BASE_H

#include <boost/monotonic/storage.hpp>

namespace boost
{
	namespace monotonic
	{
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
		extern static_storage_base<> default_static_storage;
		extern storage_base *static_storage;

		inline storage_base &get_storage()
		{
			return static_storage ? *static_storage : default_static_storage;
		}
		inline storage_base *set_storage(storage_base &store)
		{
			storage_base *old = static_storage;
			static_storage = &store;
			return old;
		}
		inline void reset_storage()
		{
			get_storage().reset();
		}
		inline void default_storage()
		{
			static_storage = &default_static_storage;
		}
		
		/// TODO: this will be specialised for 
		/// static_storage_base<..., shared_storage>, but to avoid having to link against boost::thread
		/// it is currently synonymous with unshared storage ATM

		// BOOST_MONOTONIC_SHARED_STORAGE_DEFINED is just a hack to avoid having to link with boost::thread
		// while allowing the possible inclusion of boost/monotonic/shared_storage.hpp for testing
//#ifndef BOOST_MONOTONIC_SHARED_STORAGE_DEFINED
//		extern static_storage_base<> static_shared_storage;
//#endif

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_STATIC_STORAGE_BASE_H

//EOF
