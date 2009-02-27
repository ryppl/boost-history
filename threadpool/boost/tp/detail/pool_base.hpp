//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_POOL_BASE_H
#define BOOST_TP_DETAIL_POOL_BASE_H

#include <boost/thread/tss.hpp>

#include <boost/tp/detail/worker_base.hpp>

namespace boost { namespace tp {
namespace detail
{

struct pool_base
{
	static thread_specific_ptr< worker_base >		tss_worker_;
};

thread_specific_ptr< worker_base >
pool_base::tss_worker_;

} } }

#endif // BOOST_TP_DETAIL_POOL_BASE_H

