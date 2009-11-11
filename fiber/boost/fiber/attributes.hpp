
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_ATTRIBUTES_H
#define BOOST_FIBERS_ATTRIBUTES_H

#include <cstddef>

#include <boost/fiber/detail/config.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fibers {

class BOOST_FIBER_DECL attributes
{
private:
	std::size_t		stacksize_;
	std::size_t		priority_;

public:
	attributes();

	void stack_size( std::size_t);

	std::size_t stack_size() const;

	void priority( std::size_t);

	std::size_t priority() const;
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_ATTRIBUTES_H
