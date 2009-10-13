#include "boost/tp/scanns.hpp"

namespace boost { namespace tp
{
scanns::scanns( std::size_t value)
: value_( value)
{ if ( value < 0) throw invalid_scanns("scanns must be greater than or equal to zero"); }

scanns::operator std::size_t () const
{ return value_; }
} }

