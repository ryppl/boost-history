/* test_ranlux4.cpp
 *
 * Copyright Steven Watanabe 2011
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 *
 */

#include <boost/random/ranlux.hpp>

#define BOOST_RANDOM_URNG boost::random::ranlux4

#define BOOST_RANDOM_SEED_WORDS 24

// principal operation validated with CLHEP, values by experiment
#define BOOST_RANDOM_VALIDATION_VALUE 8587295
#define BOOST_RANDOM_SEED_SEQ_VALIDATION_VALUE 6375782
#define BOOST_RANDOM_ITERATOR_VALIDATION_VALUE 4515722

#define BOOST_RANDOM_GENERATE_VALUES { 0x55E57B2CU, 0xF2DEF915U, 0x6D1A0CD9U, 0xCA0109F9U }

#include "test_generator.ipp"
