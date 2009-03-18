////////////////////////////////////////////////////////////////////////////
// utility::remove_qualifier.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_UTILITY_REMOVE_QUALIFIER_HPP_ER_2009
#define BOOST_UTILITY_REMOVE_QUALIFIER_HPP_ER_2009

#include <boost/type_traits.hpp>

namespace boost{namespace utility{

    template<typename T> //reverse order?
    struct remove_qualifier: remove_cv<
            typename remove_reference<T>::type
        >
    {};

}}
#endif // REMOVE_QUALIFIER_HPP_INCLUDED
