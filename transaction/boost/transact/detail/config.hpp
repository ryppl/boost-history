//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TRANSACT_DETAIL_CONFIG_HPP
#define BOOST_TRANSACT_DETAIL_CONFIG_HPP

#include <boost/config.hpp>

#ifdef BOOST_MSVC
#define BOOST_NESTED_OPERATOR_TEMPLATE
#else
#define BOOST_NESTED_OPERATOR_TEMPLATE template
#endif

#endif
