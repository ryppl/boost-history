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

callable::scoped_lock::scoped_lock( callable & ca, shared_ptr< thread > & thrd)
: ca_( ca)
{ ca_.impl_->set( thrd); }

callable::scoped_lock::~scoped_lock()
{ ca_.impl_->reset(); }
} } }

