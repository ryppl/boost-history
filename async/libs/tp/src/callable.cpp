#include "boost/tp/detail/callable.hpp"

namespace boost { namespace tp {
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
} } }

