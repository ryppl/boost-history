/////////////////////////////////////////////////////////////////////////////
// features.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/type_traits.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/shared_features/mpl_features.hpp>
#include <boost/shared_features/depends_on.hpp>
#include <libs/shared_features/src/example/features.h>

void example_features(){
   std::cout << "-> example_features" << std::endl;

    using namespace boost;
    namespace mpl = boost::mpl;
    namespace dep = shared_features;

    typedef dep::depends_on0                      x;
    typedef dep::depends_on1<x>                   y;
    typedef mpl::vector<x>                        vec_x;
    typedef mpl::vector<y>                        vec_y;

    typedef dep::detail::insert_feature<
        mpl::map0<>,
        vec_x
    >::type insert_x;

    BOOST_STATIC_ASSERT((
        mpl::size<insert_x>::value == 1
    ));

    typedef dep::detail::insert_dependencies<
        mpl::map0<>, y
    >::type   y_dep;

    BOOST_STATIC_ASSERT((
        mpl::size<y_dep>::value == 1
    ));

    typedef mpl::vector<y> explicit_features;
    typedef dep::detail::features<explicit_features> set;
    typedef set::vector features_as_vector;

    BOOST_STATIC_ASSERT((
        mpl::size<features_as_vector>::value == 2
    ));

    std::cout << "OK<-" << std::endl;

}
