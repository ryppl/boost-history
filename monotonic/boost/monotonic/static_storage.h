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
		template <class Storage>
		struct static_storage_base : storage_base
		{
			Storage global;

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


		extern static_storage_base<storage<> > static_storage;
		//extern static_storage_base<shared_storage<> > static_shared_storage;
	}
}

//EOF
