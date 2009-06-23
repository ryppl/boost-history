// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_STATIC_STORAGE_BASE_HPP
#define BOOST_MONOTONIC_STATIC_STORAGE_BASE_HPP

#include <boost/monotonic/storage.hpp>

namespace boost
{
	namespace monotonic
	{
		template <class Region
			, size_t InlineSize
			, size_t MinHeapIncrement
			, class Al
			, template <size_t, size_t, class> class Storage>
		struct static_storage_base
		{
			typedef Al HeapAllocator;
			typedef Storage<InlineSize, MinHeapIncrement, HeapAllocator> StorageType;

		private:
			static StorageType global;

		public:
			static_storage_base()
			{
			}
			static StorageType &get_storage()
			{
				return global;
			}
			static void reset()
			{
				global.reset();
			}
			static void release()
			{
				global.release();
			}
			static void *allocate(size_t num_bytes, size_t alignment)
			{
				return global.allocate(num_bytes, alignment);
			}
			static size_t max_size()
			{
				return global.max_size();
			}
			static size_t used()
			{
				return global.used();
			}
			static size_t remaining()
			{
				return global.remaining();
			}
		};

		/// define the static storage member for all regions
		template <class Region, size_t InlineSize, size_t MinHeapIncrement, class Al, template <size_t, size_t, class> class Storage>
		typename static_storage_base<Region, InlineSize, MinHeapIncrement, Al, Storage>::StorageType 
			static_storage_base<Region, InlineSize, MinHeapIncrement, Al, Storage>::global;

		extern storage_base *current_storage;
		template <class Region>
		inline storage_base &get_region_storage()
		{
			return static_storage_base<Region>::get_storage();
		}

		template <size_t Region>
		inline storage_base &reset_region()
		{
			return get_region_storage<Region>().reset();
		}

		template <size_t Region>
		inline storage_base &release_region()
		{
			return get_region_storage<Region>().release();
		}

		inline storage_base *set_storage(storage_base &store)
		{
			storage_base *old = current_storage;
			current_storage = &store;
			return old;
		}
		inline storage_base &get_storage()
		{
			if (current_storage)
				return *current_storage;
			return get_region_storage<default_region_tag>();
		}
		inline void default_storage()
		{
			set_storage(get_region_storage<default_region_tag>());
		}

		inline void reset_storage()
		{
			get_storage().reset();
		}

		inline void release_storage()
		{
			get_storage().release();
		}
	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_STATIC_STORAGE_BASE_HPP

//EOF
