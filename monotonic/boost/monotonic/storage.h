// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace boost
{
	namespace monotonic
	{
		/// interface common to all storage heaps
		struct storage_interface
		{
			virtual void *allocate(size_t num_bytes, void const *hint = 0) = 0;
			virtual void deallocate(void *base, size_t num_bytes) = 0;
			virtual size_t max_size() const = 0;
			virtual size_t remaining() const = 0;
		};
	}
}

//EOF
