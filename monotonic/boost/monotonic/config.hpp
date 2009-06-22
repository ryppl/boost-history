// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MONOTONIC_CONFIG_HPP
#define BOOST_MONOTONIC_CONFIG_HPP

//#define BOOST_MONOTONIC_USE_POOLS

namespace boost
{
	namespace monotonic
	{
		struct DefaultSizes
		{
			enum
			{
				InlineSize = 32*1024,						///< buffer that is inline with the storage
				MinHeapIncrement = 32*1024*1024,				///< the smallest new chunk-size for heap storage
				StaticInlineSize = 1*1024*1024,					///< inline size for a global store. this goes into your BSS
				StaticMinHeapIncrement = 32*1024*1024,
				MinPoolSize = 8,
			};
		};

	} // namespace monotonic

} // namespace boost

#endif // BOOST_MONOTONIC_CONFIG_HPP

//EOF
