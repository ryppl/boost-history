// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_CONFIG_H
#define BOOST_MONOTONIC_CONFIG_H

namespace boost
{
	namespace monotonic
	{
		struct DefaultSizes
		{
			enum
			{
				InlineSize = 8*1024,						///< buffer that is inline with the storage
				MinHeapIncrement = 4*1024*1024,				///< the smallest new chunk-size for heap storage
				StaticInlineSize = 64*1024,					///< inline size for a global store. this goes into your BSS
				StaticMinHeapIncrement = 4*1024*1024,
			};
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_CONFIG_H

//EOF
