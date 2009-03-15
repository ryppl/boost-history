#include "boost/tp/detail/guard.hpp"

#include <boost/assert.hpp>

#include <boost/tp/detail/atomic.hpp>

namespace boost { namespace tp {
namespace detail
{
guard::guard( unsigned int & active_worker)
: active_worker_( active_worker)
{
	BOOST_ASSERT( active_worker_ >= 0);
	atomic_fetch_add( & active_worker_, 1);
}

guard::~guard()
{
	atomic_fetch_sub( & active_worker_, 1);
	BOOST_ASSERT( active_worker_ >= 0);
}
} } }

