#include "boost/tp/scanns.hpp"

#include <boost/tp/exceptions.hpp>

namespace boost { namespace tp
{
scanns::scanns( std::size_t value)
: value_( value)
{ if ( value < 0) throw invalid_scanns(); }

scanns::operator std::size_t () const
{ return value_; }
} }

