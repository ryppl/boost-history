#include "boost/tp/watermark.hpp"

namespace boost { namespace tp
{
high_watermark::high_watermark( std::size_t value)
: value_( value)
{
	if ( value <= 0)
		throw invalid_watermark("high watermark must be greater than zero");
}

high_watermark::operator std::size_t () const
{ return value_; }

low_watermark::low_watermark( std::size_t value)
: value_( value)
{
	if ( value < 0)
		throw invalid_watermark("low watermark must be greater than or equal to zero");
}

low_watermark::operator std::size_t () const
{ return value_; }
} }
