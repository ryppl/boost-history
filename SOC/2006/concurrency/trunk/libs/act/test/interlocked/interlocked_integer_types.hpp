/*=============================================================================
    Copyright (c) 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_INTERLOCKED_INTEGER_TYPES_HPP
#define BOOST_ACT_TEST_INTERLOCKED_INTERLOCKED_INTEGER_TYPES_HPP

#include <boost/act/interlocked/integer/types.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/act/config/interlocked/has_int.hpp>

typedef boost::mpl::vector< 

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 8 )

boost::act::interlocked::int_fast8_t;
boost::act::interlocked::int_least8_t;

#endif

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 16 )

boost::act::interlocked::int_fast16_t;
boost::act::interlocked::int_least16_t;

#endif

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 32 )

boost::act::interlocked::int_fast32_t;
boost::act::interlocked::int_least32_t;

#endif

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 64 )

boost::act::interlocked::int_fast64_t;
boost::act::interlocked::int_least64_t;

#endif

> signed_interlocked_ints;


typedef boost::mpl::vector< 

#if BOOST_ACT_INTERLOCKED_HAS_UINT_LEAST( 8 )

boost::act::interlocked::uint_fast8_t;
boost::act::interlocked::uint_least8_t;

#endif

#if BOOST_ACT_INTERLOCKED_HAS_UINT_LEAST( 16 )

boost::act::interlocked::uint_fast16_t;
boost::act::interlocked::uint_least16_t;

#endif

#if BOOST_ACT_INTERLOCKED_HAS_UINT_LEAST( 32 )

boost::act::interlocked::uint_fast32_t;
boost::act::interlocked::uint_least32_t;

#endif

#if BOOST_ACT_INTERLOCKED_HAS_UINT_LEAST( 64 )

boost::act::interlocked::uint_fast64_t;
boost::act::interlocked::uint_least64_t;

#endif

> unsigned_interlocked_ints;

typedef boost::mpl::joint_view< signed_interlocked_ints
                              , unsigned_interlocked_ints
                              >
                              all_interlocked_ints;

#endif
