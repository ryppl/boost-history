#include "boost/tp/default_pool.hpp"
#include <boost/thread.hpp>

namespace boost { namespace tp {
namespace detail
{
default_pool
static_pool::instance = default_pool( poolsize( thread::hardware_concurrency() ) );
}
} }