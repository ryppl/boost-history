/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define BOOST_TEST_MODULE includes
#define BOOST_FUSION_TEST_NO_ADAPTED_MPL
#include "../internal/base.hpp"
#undef BOOST_FUSION_TEST_NO_ADAPTED_MPL

#include <boost/fusion/adapted/adt.hpp>
#include <boost/fusion/adapted/array.hpp>
#include <boost/fusion/adapted/boost_array.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/adapted/std_array.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/container.hpp>
#include <boost/fusion/functional.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/mpl.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/support.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/view.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>

BOOST_MPL_ASSERT_NOT((fusion::traits::is_sequence<mpl::vector<> >));

#include <boost/fusion/adapted/mpl.hpp>

//BOOST_MPL_ASSERT((fusion::traits::is_sequence<mpl::vector<> >));

BOOST_AUTO_TEST_CASE(dummy)
{}
