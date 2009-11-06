
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBER_ATTRIBUTES_H
#define BOOST_FIBER_ATTRIBUTES_H

#include <cstddef>

#include <boost/fiber/detail/config.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace fiber {

class BOOST_FIBER_DECL attributes
{
private:
	std::size_t		stacksize_;

public:
	attributes();

	void stack_size( std::size_t stacksize);

	std::size_t stack_size() const;
};

}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBER_ATTRIBUTES_H
