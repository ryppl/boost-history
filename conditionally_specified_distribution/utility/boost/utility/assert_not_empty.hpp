////////////////////////////////////////////////////////////////////////////
// utility::assert_not_empty.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_UTILITY_ASSERT_NOT_EMPTY_HPP_ER_2009
#define BOOST_UTILITY_ASSERT_NOT_EMPTY_HPP_ER_2009
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits.hpp>

namespace boost{namespace utility{

    template<typename T>
    struct assert_not_empty{
            BOOST_MPL_ASSERT((
            mpl::if_<
                is_empty<T>,
                bool_<false>,
                bool_<true>
            >
        ));
    };

#endif // BOOST_UTILITY_ASSERT_NOT_EMPTY_HPP_ER_2009
