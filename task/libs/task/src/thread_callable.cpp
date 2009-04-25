
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/thread_callable.hpp"

namespace boost { namespace task
{
namespace detail
{
void
thread_callable::operator()()
{
	impl_->run();
	impl_.reset();
}
} } }

