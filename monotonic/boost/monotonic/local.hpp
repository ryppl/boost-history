// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_LOCAL_HPP
#define BOOST_MONOTONIC_LOCAL_HPP

#include <boost/monotonic/storage.hpp>

namespace boost
{
	namespace monotonic
	{
		/// sets the global storage on construction, releases and returns to previous
		/// storage on destruction
		template <class Region, class Storage>
		struct local// : storage_base
		{
		private:
			//Storage store;
			storage_base *old;

		public:
			local()
			{
				old = set_storage(get_region_storage<Region>());
			}
			~local()
			{
				release_storage();
				if (old)
					set_storage(*old);
				else
					default_storage();
			}

			//Storage &get_storage()
			//{
			//	return store;
			//}
			//Storage const &get_storage() const
			//{
			//	return store;
			//}
			static void reset()
			{
				get_region_storage<Region>().reset();
			}
			static void release()
			{
				get_region_storage<Region>().release();
			}

			//template <class T>
			//allocator<T> make_allocator()
			//{
			//	return allocator<T>(store);
			//}

			//// the number of bytes to allocate, and the alignment to use
			//void *allocate(size_t num_bytes, size_t alignment)
			//{
			//	return store.allocate(num_bytes, alignment);
			//}

			//size_t max_size() const
			//{
			//	return store.max_size();
			//}

			//size_t used() const
			//{
			//	return store.used();
			//}

			//size_t remaining() const
			//{
			//	return store.remaining();
			//}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_LOCAL_HPP

//EOF
