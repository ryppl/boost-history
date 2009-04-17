#include "boost/task/default_pool.hpp"

#include <boost/thread.hpp>

#include "boost/task/poolsize.hpp"

namespace boost { namespace task
{
namespace detail
{
default_pool
static_pool::instance( poolsize( thread::hardware_concurrency() ) );
}
} }
