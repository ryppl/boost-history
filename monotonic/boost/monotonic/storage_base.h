// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <vector>
#include <utility>

namespace boost
{
	namespace monotonic
	{
		/// base structure for different storage types
		struct storage_base
		{
			// reset the number of bytes used to zero
			virtual void reset() = 0;

			// the number of bytes to allocate, and the alignment to use
			virtual void *allocate(size_t num_bytes, size_t alignment) = 0;
			
			virtual size_t max_size() const = 0;
			
			/// return the number of bytes used
			virtual size_t used() const = 0;

			/// return the number of bytes remaining
			virtual size_t remaining() const = 0;
		};
	}
}

//EOF
