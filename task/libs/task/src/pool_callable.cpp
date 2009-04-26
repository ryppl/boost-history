
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/pool_callable.hpp"

namespace boost { namespace task
{
namespace detail
{
pool_callable::pool_callable()
: impl_()
{}

void
pool_callable::operator()()
{ impl_->run(); }

bool
pool_callable::empty() const
{ return ! impl_; }

void
pool_callable::clear()
{ impl_.reset(); }

scoped_guard::scoped_guard( pool_callable & ca, shared_ptr< thread > & thrd)
: ca_( ca)
{ ca_.impl_->set( thrd); }

scoped_guard::~scoped_guard()
{ ca_.impl_->reset(); }
} } }

