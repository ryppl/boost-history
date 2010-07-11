/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#define FUSION_MAX_VECTOR_SIZE BOOST_FUSION_TEST_MAX_N
#define FUSION_MAX_LIST_SIZE BOOST_FUSION_TEST_MAX_N
#define FUSION_MAX_SET_SIZE BOOST_FUSION_TEST_MAX_N
#define FUSION_MAX_MAP_SIZE BOOST_FUSION_TEST_MAX_N
#define BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY BOOST_FUSION_TEST_MAX_N
#define BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY BOOST_FUSION_TEST_MAX_N
#define FUSION_MAX_ZIP_SEQUENCES BOOST_FUSION_TEST_MAX_N
#define BOOST_MPL_LIMIT_VECTOR_SIZE BOOST_FUSION_TEST_MAX_N

#include <boost/config.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/iterator.hpp>
#ifndef BOOST_FUSION_TEST_NO_ADAPTED_MPL
#   include <boost/fusion/adapted/mpl.hpp>
#endif
#include <boost/fusion/mpl.hpp>
#include <boost/fusion/support.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <iostream>
#include <string>

namespace fusion=boost::fusion;
namespace mpl=boost::mpl;

typedef mpl::range_c<int, 0, BOOST_FUSION_TEST_MAX_N> test_max_n_range;

#include "detail/types_gen.hpp"
#include "detail/rebind.hpp"
