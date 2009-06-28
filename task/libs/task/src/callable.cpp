
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/callable.hpp"

namespace boost { namespace task
{
namespace detail
{
callable::callable()
: impl_()
{}

void
callable::operator()()
{ impl_->run(); }

bool
callable::empty() const
{ return ! impl_; }

void
callable::clear()
{ impl_.reset(); }

interrupter &
callable::get_interrupter()
{ return impl_->get_interrupter(); }
} } }

