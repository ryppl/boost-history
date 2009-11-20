
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_EXCEPTIONS_H
#define BOOST_FIBERS_EXCEPTIONS_H

#include <stdexcept>
#include <string>

#include <boost/fiber/detail/config.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

class fiber_error : public std::runtime_error
{
public:
    fiber_error( std::string const& msg) :
		std::runtime_error( msg)
	{}
};

class fiber_interrupted
{};

class fiber_moved : public std::logic_error
{
public:
    fiber_moved() :
		std::logic_error("fiber moved")
	{}
};

class invalid_watermark : public std::runtime_error
{
public:
    invalid_watermark() :
		std::runtime_error("invalid watermark")
	{}
};

class lock_error : public std::logic_error
{
public:
    lock_error() :
		std::logic_error("lock invalid")
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

#endif // BOOST_FIBERS_EXCEPTIONS_H
