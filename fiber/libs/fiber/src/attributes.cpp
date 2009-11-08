
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/attributes.hpp>

#include <stdexcept>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

attributes::attributes() :
	stacksize_( 64000),
	priority_( 0)
{}

void
attributes::stack_size( std::size_t value)
{
	if ( value < 1)
		throw std::invalid_argument("invalid stacksize");
	stacksize_ = value;
}

std::size_t
attributes::stack_size() const
{ return stacksize_; }

void
attributes::priority( std::size_t value)
{ priority_ = value; }

std::size_t
attributes::priority() const
{ return priority_; }

}}

#include <boost/config/abi_suffix.hpp>
