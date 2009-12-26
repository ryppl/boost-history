
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/callable.hpp"

namespace boost {
namespace tasks {

callable::callable() :
	impl_()
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

void
callable::reset( shared_ptr< thread > const& thrd)
{ impl_->reset( thrd); }

void
callable::swap( callable & other)
{ impl_.swap( other.impl_); }

}}
