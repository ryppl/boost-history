//-----------------------------------------------------------------------------
// boost mpl/test/for_loop.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/for_loop.hpp"
#include "boost/mpl/comparison/less.hpp"
#include "boost/mpl/arithmetic/plus.hpp"
#include "boost/mpl/identity/project1st.hpp"
#include "boost/mpl/identity/project2nd.hpp"
#include "boost/mpl/compose.hpp"
#include "boost/mpl/utility.hpp"
#include "boost/mpl/et.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/static_assert.hpp"
#include <iostream>

namespace mpl = boost::mpl;

struct print_number
{
    template<typename N> struct apply
    {
        typedef N type;
        static void execute() { std::cout << N::value << '\n'; }
    };
};

int main()
{
    typedef mpl::for_loop< 
          mpl::int_t<0>
        , mpl::lt<10>
        , mpl::next<mpl::_1>
        , mpl::int_t<0>
        , mpl::plus<mpl::_1,mpl::_2>
        >::executed::state state1;
    
    typedef mpl::for_loop< 
          mpl::int_t<0>
        , mpl::lt<10>
        , mpl::next<mpl::_1>
        , state1
        , mpl::compose_f_gxy<
                mpl::prior<mpl::placeholder>
              , mpl::project1st<mpl::_1,mpl::_2>
              >
        >::executed::state state2;
    
    BOOST_STATIC_ASSERT(state1::value == 45);
    BOOST_STATIC_ASSERT(state2::value == 35);

    typedef mpl::for_loop< 
          mpl::int_t<0>
        , mpl::lt<10>
        , mpl::next<mpl::_1>
        , void
        , mpl::compose_f_gxy<
                print_number
              , mpl::project2nd<mpl::_1,mpl::_2>
              >
        > loop;

    loop::execute();
    return 0;
}
