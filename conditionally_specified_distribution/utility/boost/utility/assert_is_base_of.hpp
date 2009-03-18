////////////////////////////////////////////////////////////////////////////
// utility::assert_is_base_of.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASSERT_IS_BASE_OF_HPP_INCLUDED
#define ASSERT_IS_BASE_OF_HPP_INCLUDED
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>

namespace boost{namespace utility{

    template<typename B,typename D>
    struct assert_is_base_of{
        BOOST_MPL_ASSERT((
            is_base_of<B,D>
        ));
    };

}}

#endif // ASSERT_IS_BASE_OF_HPP_INCLUDED
