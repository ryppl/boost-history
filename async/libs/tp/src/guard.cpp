#include "boost/tp/detail/guard.hpp"

namespace boost { namespace tp {
namespace detail
{
guard::guard( volatile uint32_t & active_worker)
: active_worker_( active_worker)
{
	BOOST_ASSERT( active_worker_ >= 0);
	interprocess::detail::atomic_inc32( & active_worker_);
}

guard::~guard()
{
	interprocess::detail::atomic_dec32( & active_worker_);
	BOOST_ASSERT( active_worker_ >= 0);
}
} } }

