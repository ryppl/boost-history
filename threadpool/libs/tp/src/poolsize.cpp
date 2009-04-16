#include "boost/tp/poolsize.hpp"

#include <boost/tp/exceptions.hpp>

namespace boost { namespace tp
{
poolsize::poolsize( std::size_t value)
: value_( value)
{ if ( value <= 0) throw invalid_poolsize(); }

poolsize::operator std::size_t () const
{ return value_; }
} }

