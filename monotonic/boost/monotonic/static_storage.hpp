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
		namespace detail
		{
			template <>
			struct storage_type<default_access_tag>
			{
				template <size_t N, size_t M, class Al>
				struct storage
				{
					typedef monotonic::storage<N,M,Al> type;
				};
			};
		}

		template <class Region
			, class Access
			, size_t InlineSize
			, size_t MinHeapIncrement
			, class Al>
		struct static_storage_base
		{
			typedef Al HeapAllocator;
			typedef detail::storage_type<Access> Selector;
			typedef typename Selector::template storage<InlineSize, MinHeapIncrement, HeapAllocator>::type StorageType;

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

		/// define the static storage member for all regions with all access types
		template <class Region
			, class Access
			, size_t InlineSize
			, size_t MinHeapIncrement
			, class Al>
		typename static_storage_base<Region, Access, InlineSize, MinHeapIncrement, Al>::StorageType 
			static_storage_base<Region, Access, InlineSize, MinHeapIncrement, Al>::global;

		template <class Region, class Access>
		inline storage_base &get_storage()
		{
			return static_storage_base<Region,Access>::get_storage();
		}

		inline void reset_storage()
		{
			get_storage<default_region_tag, default_access_tag>().reset();
		}

		inline void release_storage()
		{
			get_storage<default_region_tag, default_access_tag>().release();
		}

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_STATIC_STORAGE_BASE_HPP

//EOF
