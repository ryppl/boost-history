#include "boost/tp/poolsize.hpp"

namespace boost { namespace tp
{
poolsize::poolsize( std::size_t value)
: value_( value)
{ if ( value <= 0) throw invalid_poolsize("core poolsize must be greater than zero"); }

poolsize::operator std::size_t () const
{ return value_; }
} }

