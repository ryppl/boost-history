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
		template <class Region, class Access = default_access_tag>
		struct local
		{
			local()
			{
			}
			~local()
			{
				release();
			}
			static void reset()
			{
				get_storage().reset();
			}
			static void release()
			{
				get_storage().release();
			}
			static storage_base &get_storage()
			{
				return ::boost::monotonic::get_storage<Region,Access>();
			}
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_LOCAL_HPP

//EOF
