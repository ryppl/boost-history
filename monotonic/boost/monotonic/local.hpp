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
		/// RIIA for storage
		template <size_t InlineSize
			, class Region
			, size_t MinHeapIncrement
			, class Access
			, class Al
		>
		struct local
		{
			typedef static_storage<Region,Access,InlineSize,MinHeapIncrement,Al> StaticStorage;

			local()
			{
			}
			~local()
			{
				reset();
			}
			static typename StaticStorage::StorageType &get_storage()
			{
				return StaticStorage::get_storage();
			}
			static void reset()
			{
				get_storage().reset();
			}
			static void release()
			{
				get_storage().release();
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_LOCAL_HPP

//EOF
