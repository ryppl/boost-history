
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBER_EXCEPTIONS_H
#define BOOST_FIBER_EXCEPTIONS_H

#include <stdexcept>
#include <string>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

class fiber_error : public std::runtime_error
{
public:
    fiber_error( std::string const& msg) :
		std::runtime_error( msg)
	{}
};

class invalid_stacksize : public std::runtime_error
{
public:
    invalid_stacksize() :
		std::runtime_error("invalid stacksize")
	{}
};

class scheduler_error : public std::runtime_error
{
public:
    scheduler_error( std::string const& msg) :
		std::runtime_error( msg)
	{}
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBER_EXCEPTIONS_H
