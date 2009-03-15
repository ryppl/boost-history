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
	atomic_inc_32( & active_worker_);
}

guard::~guard()
{
	atomic_dec_32( & active_worker_);
	BOOST_ASSERT( active_worker_ >= 0);
}
} } }

