#include "boost/task/poolsize.hpp"

#include <boost/task/exceptions.hpp>

namespace boost { namespace task
{
poolsize::poolsize( std::size_t value)
: value_( value)
{ if ( value <= 0) throw invalid_poolsize(); }

poolsize::operator std::size_t () const
{ return value_; }
} }

