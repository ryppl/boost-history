/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#include <boost/mpl_ext/and_seq.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/bool.hpp>


namespace bldsl = ::boost::mpl_ext;
using namespace boost::mpl;
struct unknown;

//____________________________________________________________________________//

BOOST_STATIC_ASSERT((bldsl::and_seq<vector<true_> >::value));
BOOST_STATIC_ASSERT((bldsl::and_seq<vector<true_, true_> >::value));
BOOST_STATIC_ASSERT((bldsl::and_seq<vector<true_, true_, true_, true_, true_> >::value));
BOOST_STATIC_ASSERT((not_<bldsl::and_seq<vector<true_, false_> > >::value));
BOOST_STATIC_ASSERT((not_<bldsl::and_seq<vector<false_, false_> > >::value));
BOOST_STATIC_ASSERT((not_<bldsl::and_seq<vector<false_, true_> > >::value));
BOOST_STATIC_ASSERT((not_<bldsl::and_seq<vector<false_, unknown, false_> > >::value));



//static void test(void)
//{
//}

//____________________________________________________________________________//

