//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_BIND_PROCESSOR_IRIX_H
#define BOOST_TP_DETAIL_BIND_PROCESSOR_IRIX_H

extern "C"
{
#include <pthread.h>
}

#include <boost/assert.hpp>
#include <boost/thread.hpp>
#include <boost/system/system_error.hpp>

namespace boost { namespace this_thread
{
	inline
	void bind_to_processor( unsigned int n)
	{
		BOOST_ASSERT( n >= 0);
		BOOST_ASSERT( n < boost::thread::hardware_concurrency() );

		int errno_( ::pthread_setrunon_np( n) );
		if ( errno_ != 0)
			throw boost::system::system_error(
					boost::system::error_code(
						errno_,
						boost::system::system_category) );
	}

	// bind_to_any_processor() can not be supported
}}

#endif // BOOST_TP_DETAIL_BIND_PROCESSOR_IRIX_H
