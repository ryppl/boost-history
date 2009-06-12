// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/cstdint.hpp>	
#include <boost/monotonic/storage.h>

namespace boost
{
	namespace monotonic
	{
		/// base structure for all allocators. 
		/// 
		/// implements storage_interface using malloc and free
		struct storage_base : storage_interface
		{
			void *allocate(size_t num_bytes, void const * = 0)
			{
				return malloc(num_bytes);
			}

			void deallocate(void *p, size_t /*n*/)
			{
				free(p);
			}

			size_t max_size() const
			{
				return INT_MAX;
			}
		};
	}
}

//EOF
