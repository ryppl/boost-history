
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/guard.hpp"

#include <boost/assert.hpp>

namespace boost {
namespace tasks {
namespace detail {

guard::guard( atomic< unsigned int > & active_worker) :
	active_worker_( active_worker)
{
	BOOST_ASSERT( 0 <= active_worker_.load() );
	active_worker_.fetch_add( 1);
}

guard::~guard()
{
	active_worker_.fetch_sub( 1);
	BOOST_ASSERT( 0 <= active_worker_.load() );
}

}}}
