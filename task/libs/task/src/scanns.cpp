#include "boost/task/scanns.hpp"

#include <boost/task/exceptions.hpp>

namespace boost { namespace task
{
scanns::scanns( std::size_t value)
: value_( value)
{ if ( value < 0) throw invalid_scanns(); }

scanns::operator std::size_t () const
{ return value_; }
} }

