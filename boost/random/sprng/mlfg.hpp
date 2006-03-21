/* 
 * Copyright Matthias Troyer 2006
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
*
 */

#ifndef BOOST_RANDOM_SPRNG_MLFG_HPP
#define BOOST_RANDOM_SPRNG_MLFG_HPP


#define BOOST_SPRNG_GENERATOR   mlfg
#define BOOST_SPRNG_TYPE        4
#define BOOST_SPRNG_MAX_STREAMS 0x7fffffff
#define BOOST_SPRNG_MAX_PARAMS  11
#define BOOST_SPRNG_VALIDATION 0.77628973267217871168

#include <boost/random/sprng/detail/implementation.hpp>

#undef BOOST_SPRNG_GENERATOR
#undef BOOST_SPRNG_TYPE
#undef BOOST_SPRNG_MAX_STREAMS
#undef BOOST_SPRNG_MAX_PARAMS
#undef BOOST_SPRNG_VALIDATION

#endif // BOOST_RANDOM_SPRNG_MLFG_HPP
