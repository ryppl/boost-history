
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBER_EXCEPTIONS_H
#define BOOST_FIBER_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace boost {
namespace fiber {

class scheduler_error : public std::runtime_error
{
public:
    scheduler_error( std::string const& msg)
	: std::runtime_error( msg)
	{}
};

}}

#endif // BOOST_FIBER_EXCEPTIONS_H
