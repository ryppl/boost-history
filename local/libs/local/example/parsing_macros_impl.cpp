
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[parsing_macros_impl_cpp
#include <boost/detail/preprocessor/keyword/this.hpp>
#include <boost/detail/preprocessor/keyword/const.hpp>
#include <boost/local/aux_/preprocessor/keyword/bind.hpp>
#include <cassert>

int main() {
    // Detect `this`.
    assert( BOOST_DETAIL_PP_KEYWORD_IS_THIS_BACK(
            BOOST_LOCAL_AUX_PP_KEYWORD_BIND_REMOVE_FRONT(
            BOOST_DETAIL_PP_KEYWORD_CONST_REMOVE_FRONT(
                const bind this
            ))) == 1 );
    assert( BOOST_DETAIL_PP_KEYWORD_IS_THIS_BACK(
            BOOST_LOCAL_AUX_PP_KEYWORD_BIND_REMOVE_FRONT(
            BOOST_DETAIL_PP_KEYWORD_CONST_REMOVE_FRONT(
                const bind& x
            ))) == 0 );

    return 0;
}
//]

